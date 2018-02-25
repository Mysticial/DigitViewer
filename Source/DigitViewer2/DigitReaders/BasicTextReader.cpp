/* BasicTextReader.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include <algorithm>
#include "PublicLibs/Exceptions/StringException.h"
#include "PublicLibs/Exceptions/BufferTooSmallThrower.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/BasicLibs/Concurrency/BlockSplitting.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "DigitViewer2/DigitHash/DigitHash.h"
#include "DigitViewer2/RawToAscii/RawToAscii.h"
#include "BasicTextReader.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char BasicTextReader::auto_detect_radix(){
    //  Assume radix 10 until a hexadecimal digit is found.
    char radix = 10;

    //  Read the first 64 bytes to guess the radix.
    m_file.set_ptr(0);

    for (upL_t c = 0; c < 64; c++){
        char ch;
        if (m_file.read(&ch, 1) != 1){
            break;
        }

        //  Skip the decimal place.
        if (ch == '.'){
            continue;
        }

        //  Decimal digit
        if ('0' <= ch && ch <= '9'){
            continue;
        }

        //  Hexadecimal digit
        if ('a' <= ch && ch <= 'f'){
            radix = 16;
            break;
        }

        throw StringException("TextReader::auto_detect_radix()", "Invalid Digit");
    }

    return radix;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicTextReader::BasicTextReader(const std::string& path, char radix)
    : m_file(path)
{
    //  Auto-detect radix
    m_radix = radix == 0
        ? auto_detect_radix()
        : radix;

    //  Find the decimal place
    m_file.set_ptr(0);
    ufL_t c = 0;
    while (1){
        char ch;
        if (m_file.read(&ch, 1) != 1){
            throw FileIO::FileException(
                "BasicTextReader::BasicTextReader()",
                path,
                "Unexpected End of File"
            );
        }

        c++;
        if (ch == '.'){
            m_dp_offset = c;
            break;
        }
        if (c == 63){
            throw FileIO::FileException(
                "BasicTextReader::BasicTextReader()",
                path,
                "A decimal place was not found within the first 63 bytes of the file."
            );
        }
    }

    //  # of digits after decimal place.
    m_total_digits = FileIO::GetFileSize(path.c_str()) - m_dp_offset;

    switch (m_radix){
    case 10:
        m_fp_convert = RawToAscii::dec_to_raw;
        break;
    case 16:
        m_fp_convert = RawToAscii::hex_to_raw;
        break;
    default:
        throw InvalidParametersException("BasicTextReader::BasicTextReader()", "Invalid Radix");
    }
}
std::string BasicTextReader::first_digits(){
    const upL_t DIGITS = 64;

    std::lock_guard<std::mutex> lg(m_lock);
    m_file.set_ptr(0);

    std::string str(DIGITS, '\0');
    m_file.read(&str[0], DIGITS);

    return std::string(str.c_str());
}
bool BasicTextReader::range_is_available(uiL_t offset, uiL_t digits){
    return offset + digits <= m_total_digits;
}
upL_t BasicTextReader::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    upL_t bytes = (upL_t)std::min(digits, (uiL_t)limit);
    bytes = std::max(bytes, DEFAULT_ALIGNMENT);
    return bytes;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicTextReader::Action_process : public BasicAction{
    const BasicTextReader& m_object;
    DigitStats* m_stats;
    char* m_buffer;
    upL_t m_digits;

    upL_t m_unit_L;

public:
    Action_process(
        const BasicTextReader& object,
        DigitStats* stats,
        char* buffer, upL_t digits,
        upL_t unit_L
    )
        : m_object(object)
        , m_stats(stats)
        , m_buffer(buffer), m_digits(digits)
        , m_unit_L(unit_L)
    {}

    virtual void run(upL_t index) override{
        upL_t si = m_unit_L * index;
        upL_t ei = si + m_unit_L;
        if (si >= m_digits){
            return;
        }
        if (ei > m_digits){
            ei = m_digits;
        }

        DigitStats* stats = m_stats == nullptr
            ? nullptr
            : m_stats + index;

        m_object.process(stats, m_buffer + si, ei - si);

        if (stats != nullptr){
            stats->scale_up_hash(m_digits - ei);
        }
    }
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicTextReader::process(DigitStats* stats, char* buffer, upL_t digits) const{
    //  Optimization: If "buffer" is misaligned, do enough to align it.
    upL_t align = Alignment::ptr_to_aligned<DEFAULT_ALIGNMENT>(buffer);
    if (digits > DEFAULT_ALIGNMENT && align != 0){
        process(stats, buffer, align);
        buffer += align;
        digits -= align;
    }

    if (stats == nullptr){
        if (m_fp_convert(buffer, buffer, digits)){
            throw InvalidParametersException("BasicTextReader::process_txt()", "Invalid Digit");
        }
        return;
    }

    while (digits > 0){
        upL_t block = std::min(digits, (upL_t)CACHE_BLOCK);

        if (m_fp_convert(buffer, buffer, block)){
            throw InvalidParametersException("BasicTextReader::process_txt()", "Invalid Digit");
        }
        if (stats != nullptr){
            stats->accumulate(buffer, block);
        }

        buffer += block;
        digits -= block;
    }

    if (stats == nullptr){
        return;
    }

    alignas(DEFAULT_ALIGNMENT) char cache[CACHE_BLOCK];
    while (digits > 0){
        upL_t block = std::min(digits, (upL_t)CACHE_BLOCK);

        if (m_fp_convert(cache, buffer, block)){
            throw InvalidParametersException("BasicTextReader::process_txt()", "Invalid Digit");
        }
        if (stats != nullptr){
            stats->accumulate(cache, block);
        }

        buffer += block;
        digits -= block;
    }
}
void BasicTextReader::process(
    DigitStats* stats, char* buffer, upL_t digits,
    BasicParallelizer& parallelizer, upL_t tds
) const{
    //  Optimization: If "buffer" is misaligned, do enough to align it.
    upL_t align = Alignment::ptr_to_aligned<DEFAULT_ALIGNMENT>(buffer);
    if (digits > DEFAULT_ALIGNMENT && align != 0){
        process(stats, buffer, align);
        buffer += align;
        digits -= align;
    }

    const upL_t THRESHOLD = 10000;

    if (digits < THRESHOLD || tds <= 1){
        process(stats, buffer, digits);
        return;
    }

    upL_t unit_L = parallel_split_words_aligned<char, DEFAULT_ALIGNMENT>(
        digits, tds, THRESHOLD
    );

    std::vector<DigitStats> vstats;
    if (stats != nullptr){
        vstats.emplace_back(m_radix, stats->hash());
        for (upL_t c = 1; c < tds; c++){
            vstats.emplace_back(m_radix);
        }
    }

    Action_process action(
        *this,
        stats != nullptr ? &vstats[0] : nullptr,
        buffer, digits, unit_L
    );
    parallelizer.run_in_parallel(action, 0, tds);

    if (stats != nullptr){
        stats[0].clear_hash();
        for (const DigitStats& stat : vstats){
            stats[0] += stat;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicTextReader::load_stats(
    DigitStats& stats,
    uiL_t offset, uiL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Ends past the end.
    uiL_t end = offset + digits;
    if (end > m_total_digits){
        throw StringException("BasicTextReader::load_stats()", "Out of range.");
    }

    char* buffer = (char*)P;
    if (Alignment::ptr_past_aligned<DEFAULT_ALIGNMENT>(buffer) != 0){
        throw InvalidParametersException("BasicTextReader::load_stats()", "Buffer is misaligned.");
    }

    Alignment::align_int_down_inplace<DEFAULT_ALIGNMENT>(Pbytes);
    check_BufferTooSmall("BasicTextReader::load_stats()", Pbytes, DEFAULT_ALIGNMENT);

    while (digits > 0){
        upL_t block = (upL_t)std::min(digits, (uiL_t)Pbytes);

        //  Read from file.
        {
            std::lock_guard<std::mutex> lg(m_lock);
            m_file.set_ptr(m_dp_offset + static_cast<ufL_t>(offset));
            upL_t bytes = m_file.read(buffer, block);
            if (bytes != block){
                throw StringException("BasicTextReader::load_stats()", "Error Reading from File");
            }
        }

        process(&stats, buffer, block, parallelizer, tds);
        offset += block;
        digits -= block;
    }
}
void BasicTextReader::load_digits(
    char* output,
    DigitStats* stats,
    uiL_t offset, upL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Ends past the end.
    uiL_t end = offset + digits;
    if (end > m_total_digits){
        throw StringException("BasicTextReader::load_digits()", "Out of range.");
    }

    //  Read from file.
    {
        std::lock_guard<std::mutex> lg(m_lock);
        m_file.set_ptr(m_dp_offset + static_cast<ufL_t>(offset));
        if (m_file.read(output, digits) != digits){
            throw StringException("BasicTextReader::load_digits()", "Error Reading from File");
        }
    }

    process(stats, output, digits, parallelizer, tds);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
