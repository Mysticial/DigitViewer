/* BasicYcdFileReader.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/29/2018
 * Last Modified    : 01/29/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <vector>
#include <algorithm>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/BufferTooSmallThrower.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/BasicLibs/Concurrency/BlockSplitting.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "DigitViewer2/RawToCompressed/RawToCompressed.h"
#include "DigitViewer/Globals.h"
#include "DigitViewer/DigitReaders/ParsingTools.h"
#include "BasicYcdFileReader.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicYcdFileReader::BasicYcdFileReader(const std::string& path)
    : m_path(path)
    , m_file(path)
{
    //  Parse the file

    using namespace DigitViewer;

    //  Start after the next newline.
    {
        char ch;
        do{
            if (m_file.read(&ch, 1) == 0){
                FileIO::PrintLastError();
                throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, "Invalid File Format");
            }
        }while (ch != '\n');
    }

    //  Parse header info
    while (1){
        std::string token = grab_until_delim(&m_file, '\n');

        //  Empty line
        if (token.size() == 0){
            continue;
        }

        //  End of header section
        if (token == YCF_CDF_TOKEN_EndHeader){
            break;
        }

        //  Break up the token.
        std::string key, value;

        //  Parse the key
        const char* ptr = token.c_str();
        ptr = grab_until_delim(key, ptr, '\t');

        //  Parse the value
        grab_until_delim(value, ptr, '\n');

        //  file_version
        if (key == YCF_CDF_TOKEN_FileVersion){
            m_file_version = std::move(value);
            continue;
        }

        //  radix
        if (key == YCF_CDF_TOKEN_Base){
            m_radix = (char)parse_uL(value.c_str());
            continue;
        }

        //  first_digits
        if (key == YCF_CDF_TOKEN_FirstDigits){
            m_first_digits = std::move(value);
            continue;
        }

        //  stream_end
        if (key == YCF_CDF_TOKEN_TotalDigits){
            m_stream_end = (uiL_t)parse_uL(value.c_str());
            continue;
        }

        //  digits_per_file
        if (key == YCF_CDF_TOKEN_BlockSize){
            m_digits_per_file = (ufL_t)parse_uL(value.c_str());
            continue;
        }

        //  file_id
        if (key == YCF_CDF_TOKEN_BlockID){
            m_file_id = parse_uL(value.c_str());
            continue;
        }
    }

    //  Find offset
    //  This isn't efficient. It sets the file pointer back to zero and counts
    //  to the first null character.
    m_file.set_ptr(0);
    char ch;
    ufL_t c = 0;
    while (1){
        if (m_file.read(&ch, 1) == 0){
            throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, "Error Reading File");
        }
        c++;
        if (ch == '\0')
            break;
    };
    m_data_offset = c;

    //  Check Version
    if (m_file_version.size() == 0){
        throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, "No version # found.");
    }
    if (m_file_version != "1.0.0" && m_file_version != "1.1.0"){
        throw FileIO::FileException(
            "YCDFileReader::YCDFileReader()",
            path,
            "This .ycd file is of a later format version.\n"
            "This version of the digit viewer is unable to view this file."
        );
    }

    //  Other checks
    if (m_digits_per_file < 100){
        throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, "Invalid Digits per File");
    }

    if (m_stream_end != 0 && m_digits_per_file > m_stream_end){
        m_digits_per_file = static_cast<ufL_t>(m_stream_end);
    }

    //  Boundaries
    uiL_t block_start = m_file_id * m_digits_per_file;
    uiL_t block_end   = block_start + m_digits_per_file;
    if (m_stream_end != 0){
        if (m_stream_end <= block_start){
            std::string error = "This file is out of range.\n";
            error += "Start:        ";
            error += std::to_string(block_start);
            error += "\n";
            error += "End:          ";
            error += std::to_string(block_end);
            error += "\n";
            error += "Total Digits: ";
            error += std::to_string(m_stream_end);
            error += "\n";
            throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, error);
        }
        if (block_end > m_stream_end){
            block_end = m_stream_end;
        }
    }
    ufL_t block_digits = (ufL_t)(block_end - block_start);

    //  Radix-specific things
    switch (m_radix){
        case 10:
            m_digits_per_word = 19;
            m_words_in_this_file = (block_digits - 1) / 19 + 1;
            m_fp_convert = RawToCompressed::i64_to_dec;
            break;
        case 16:
            m_digits_per_word = 16;
            m_words_in_this_file = (block_digits - 1) / 16 + 1;
            m_fp_convert = RawToCompressed::i64_to_hex;
            break;
        default:
            throw FileIO::FileException("BasicYcdFileReader::BasicYcdFileReader()", path, "Unsupported Radix");
    }
}
bool BasicYcdFileReader::range_is_available(uiL_t offset, uiL_t digits){
    uiL_t s = m_file_id * m_digits_per_file;
    uiL_t e = s + m_digits_per_file;
    if (m_stream_end != 0){
        e = std::min(e, m_stream_end);
    }
    return offset >= s && offset + digits <= e;
}
upL_t BasicYcdFileReader::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    if (digits == 0){
        return 0;
    }
    uiL_t words = (digits - 1) / m_digits_per_word + 1;
    upL_t bytes = (upL_t)std::min((words + 2) * sizeof(u64_t), (uiL_t)limit);
    bytes = std::max(bytes, DEFAULT_ALIGNMENT + 2 * sizeof(u64_t));
    return bytes;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicYcdFileReader::print() const{
    Console::println(m_path);
    Console::println();

    Console::println_labelm("file_version:", m_file_version);
    Console::println_labelm("radix:", m_radix);
    Console::println_labelm("first_digits:", m_first_digits);
    Console::println_labelm_commas("stream_end:", m_stream_end);
    Console::println_labelm_commas("digits_per_file:", m_digits_per_file);
    Console::println_labelm_commas("file_id:", m_file_id);
    Console::println();

    Console::println_labelm_commas("digits_per_word:", m_digits_per_word);
    Console::println_labelm_commas("words_in_this_file:", m_words_in_this_file);
    Console::println_labelm_commas("data_offset:", m_data_offset);
    Console::println();
}
void BasicYcdFileReader::read_words(ufL_t pos, u64_t* T, upL_t L){
    //  This method reads L words starting at offset "pos".
    //  "pos" is measured in 64-bit words. It is relative to the start of the
    //  data section of the .ycd file.

    //  Thus the byte region of the file that is read is:
    //      [
    //          (data section offset) + pos * sizeof(u64_t),
    //          (data section offset) + (pos + L) * sizeof(u64_t)
    //      )

    //  Note the [start, end). Thus it reads from "start" and ends 1 byte before
    //  "end".

    //  If any portion is out of range of the file, it throw an exception.

    if (pos + L > m_words_in_this_file){
        Console::Warning("Internal Error: Read out of Bounds");
        std::string error = "BasicYcdFileReader::read_words()\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(pos);
        error += " - ";
        error += std::to_string(pos + L);
        error += "\nAvailable Range: ";
        error += std::to_string(0);
        error += " - ";
        error += std::to_string(m_words_in_this_file);
        throw FileIO::FileException("BasicYcdFileReader::read_words()", m_path, error);
    }

    std::lock_guard<std::mutex> lg(m_lock);

    //  Set file pointer
    m_file.set_ptr(m_data_offset + pos * sizeof(u64_t));

    //  Read
    upL_t words_read = m_file.read(T, L * sizeof(u64_t)) / sizeof(u64_t);
    if (words_read != L){
        throw FileIO::FileException("BasicYcdFileReader::read_words()", m_path, "Error Reading File");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicYcdFileReader::process_blocks(
    DigitStats& stats,
    u64_t* B, upL_t BL
) const{
    alignas(DEFAULT_ALIGNMENT) char raw_digits[19 * CACHE_BLOCK];
    while (BL > 0){
        upL_t current_blocks = std::min(BL, CACHE_BLOCK);
        upL_t current_digits = current_blocks * m_digits_per_word;

        m_fp_convert(raw_digits, B, current_blocks);
        stats.accumulate(raw_digits, current_digits);

        B += current_blocks;
        BL -= current_blocks;
    }
}
void BasicYcdFileReader::process_blocks(
    char* raw_digits,
    DigitStats* stats,
    u64_t* B, upL_t BL
) const{
    if (stats == nullptr){
        //  Single-pass only. No cache blocking needed.
        m_fp_convert(raw_digits, B, BL);
    }else{
        //  Cache block this.
        while (BL > 0){
            upL_t current_blocks = std::min(BL, CACHE_BLOCK);
            upL_t current_digits = current_blocks * m_digits_per_word;

            m_fp_convert(raw_digits, B, current_blocks);
            stats->accumulate(raw_digits, current_digits);

            raw_digits += current_digits;
            B += current_blocks;
            BL -= current_blocks;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicYcdFileReader::Action_process : public BasicAction{
    const BasicYcdFileReader& m_object;
    char* m_raw_digits;
    DigitStats* m_stats;
    u64_t* m_B;
    upL_t m_BL;

    upL_t m_unit_L;

public:
    Action_process(
        const BasicYcdFileReader& object,
        char* raw_digits,
        DigitStats* stats,
        u64_t* B, upL_t BL,
        upL_t unit_L
    )
        : m_object(object)
        , m_raw_digits(raw_digits)
        , m_stats(stats)
        , m_B(B), m_BL(BL)
        , m_unit_L(unit_L)
    {}

    virtual void run(upL_t index) override{
        upL_t si = m_unit_L * index;
        upL_t ei = si + m_unit_L;
        if (si >= m_BL){
            return;
        }
        if (ei > m_BL){
            ei = m_BL;
        }

        DigitStats* stats = m_stats == nullptr
            ? nullptr
            : m_stats + index;

        if (m_raw_digits == nullptr){
            m_object.process_blocks(*stats, m_B + si, ei - si);
        }else{
            m_object.process_blocks(m_raw_digits + si * m_object.m_digits_per_word, stats, m_B + si, ei - si);
        }

        if (stats != nullptr){
            stats->scale_up_hash((m_BL - ei) * m_object.m_digits_per_word);
        }
    }
};
void BasicYcdFileReader::process_blocks(
    char* raw_digits,
    DigitStats* stats,
    u64_t* B, upL_t BL,
    BasicParallelizer& parallelizer, upL_t tds
) const{
    const upL_t THRESHOLD = 1000;

    if (BL < THRESHOLD || tds <= 1){
        if (raw_digits == nullptr){
            process_blocks(*stats, B, BL);
        }else{
            process_blocks(raw_digits, stats, B, BL);
        }
        return;
    }

    //  Align to both DEFAULT_ALIGNMENT as well as m_digits_per_word.
    upL_t unit_L = parallel_split_aligned(
        BL, tds,
        DEFAULT_ALIGNMENT * m_digits_per_word,
        THRESHOLD
    );

//    cout << "tds    = " << tds << endl;
//    cout << "unit_L = " << unit_L << endl;

    std::vector<DigitStats> vstats;
    if (stats != nullptr){
        vstats.emplace_back(m_radix, stats->hash());
        for (upL_t c = 1; c < tds; c++){
            vstats.emplace_back(m_radix);
        }
    }

    Action_process action(
        *this,
        raw_digits,
        stats != nullptr ? &vstats[0] : nullptr,
        B, BL,
        unit_L
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
void BasicYcdFileReader::load_stats_B(
    DigitStats& stats,
    uiL_t offset, upL_t digits,
    u64_t* P, upL_t PL,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Conditions:
    //   -  digits <= (PL - 2) * m_digits_per_word

    if (digits == 0){
        return;
    }

    ufL_t end = (ufL_t)offset + digits;
    ufL_t word_s = (ufL_t)offset / m_digits_per_word;
    ufL_t word_e = end / m_digits_per_word;

    bool end_tail = word_e * m_digits_per_word < end;

    //  Read into memory.
    {
        ufL_t read_s = word_s;
        ufL_t read_e = end_tail ? word_e + 1 : word_e;
        upL_t read_L = (upL_t)(read_e - read_s);
        read_words(word_s, P, read_L);
//        Console::println_af(P, read_L);
    }

//    alignas(DEFAULT_ALIGNMENT) char raw_digits[19 * CACHE_BLOCK];

    //  Start Filter
    ufL_t read_s = word_s * m_digits_per_word;
    if (read_s != offset){
//        cout << "Start Filter" << endl;
        char buffer[19];
        m_fp_convert(buffer, P, 1);
        P += 1;

        //  Compute sizes
        upL_t diff = (upL_t)(offset - read_s);
        upL_t size = m_digits_per_word - diff;

        //  Make sure we don't overrun the end.
        size = std::min(size, digits);

        stats.accumulate(buffer + diff, size);

        digits -= size;
        word_s++;
    }

    if (digits == 0){
        return;
    }

    //  Steady State
    upL_t blocks = (upL_t)(word_e - word_s);
    if (blocks > 0){
//        cout << "Steady" << endl;
        process_blocks(nullptr, &stats, P, blocks, parallelizer, tds);

        upL_t current_digits = blocks * m_digits_per_word;
        P += blocks;
        digits -= current_digits;
        word_s += blocks;
    }

    //  End Filter
    if (end_tail){
//        cout << "End Filter" << endl;
        char buffer[19];
        m_fp_convert(buffer, P, 1);

        stats.accumulate(buffer, digits);
    }
}
void BasicYcdFileReader::load_digits_B(
    char* output,
    DigitStats* stats,
    uiL_t offset, upL_t digits,
    u64_t* P, upL_t PL,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Conditions:
    //   -  digits <= (PL - 2) * m_digits_per_word

    if (digits == 0){
        return;
    }

    ufL_t end = (ufL_t)offset + digits;
    ufL_t word_s = (ufL_t)offset / m_digits_per_word;
    ufL_t word_e = end / m_digits_per_word;

    bool end_tail = word_e * m_digits_per_word < end;

    //  Read into memory.
    {
        ufL_t read_s = word_s;
        ufL_t read_e = end_tail ? word_e + 1 : word_e;
        upL_t read_L = (upL_t)(read_e - read_s);
        read_words(word_s, P, read_L);
//        Console::println_af(P, read_L);
    }

    //  Start Filter
    ufL_t read_s = word_s * m_digits_per_word;
    if (read_s != offset){
//        cout << "Start Filter" << endl;
        char buffer[19];
        m_fp_convert(buffer, P, 1);
        P += 1;

        //  Compute sizes
        upL_t diff = (upL_t)(offset - read_s);
        upL_t size = m_digits_per_word - diff;

        //  Make sure we don't overrun the end.
        size = std::min(size, digits);

        //  Copy to output.
        memcpy(output, buffer + diff, size);
        if (stats != nullptr){
            stats->accumulate(output, size);
        }

        output += size;
        digits -= size;
        word_s++;
    }

    if (digits == 0){
        return;
    }


    //  Steady State
    upL_t blocks = (upL_t)(word_e - word_s);
    if (blocks > 0){
//        cout << "Steady" << endl;
        process_blocks(output, stats, P, blocks, parallelizer, tds);

        upL_t current_digits = blocks * m_digits_per_word;
        P += blocks;
        output += current_digits;
        digits -= current_digits;
        word_s += blocks;
    }

    //  End Filter
    if (end_tail){
//        cout << "End Filter" << endl;
        char buffer[19];
        m_fp_convert(buffer, P, 1);
//        cout << "digits = " << digits << endl;

        //cout << P[1] << endl;
//        cout << P[0] << endl;
//        for (upL_t c = 0; c < digits; c++){
//            cout << "digit = " << (int)buffer[c] << endl;
//        }

        memcpy(output, buffer, digits);
        if (stats != nullptr){
            stats->accumulate(output, digits);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicYcdFileReader::load_stats(
    DigitStats& stats,
    uiL_t offset, uiL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (digits == 0){
        return;
    }

    //  Get boundaries
    uiL_t block_start = m_digits_per_file * m_file_id;
    uiL_t block_end   = block_start + m_digits_per_file;
    if (m_stream_end != 0 && block_end > m_digits_per_file){
        block_end = m_stream_end;
    }

    //  Check boundaries
    if (offset + digits > block_end || offset < block_start){
        std::string error = "void BasicYcdFileReader::load_digits()\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(offset);
        error += " - ";
        error += std::to_string(offset + digits);
        error += "\nAvailable Range: ";
        error += std::to_string(block_start);
        error += " - ";
        error += std::to_string(block_end);
        throw FileIO::FileException("BasicYcdFileReader::load_digits()", m_path, error);
    }

    if (Alignment::int_past_aligned<DEFAULT_ALIGNMENT>((upL_t)P) != 0){
        throw InvalidParametersException("BasicTextReader::load_digits()", "Buffer is misaligned.");
    }

    upL_t PL = Pbytes / sizeof(u64_t);
    check_BufferTooSmall("BasicYcdFileReader::load_digits()", PL, DEFAULT_ALIGNMENT / sizeof(u64_t) + 2);

    offset -= block_start;
    while (digits > 0){
        upL_t current_digits = (upL_t)std::min(digits, (uiL_t)((PL - 2) * m_digits_per_word));

        load_stats_B(stats, offset, current_digits, (u64_t*)P, PL, parallelizer, tds);

        offset += current_digits;
        digits -= current_digits;
    }
}
void BasicYcdFileReader::load_digits(
    char* output,
    DigitStats* stats,
    uiL_t offset, upL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  This method reads digits from the file.
    //  It reads the following region:
    //      [pos, pos + digits)

    //  "pos" is relative to the start of the constant. Thus it is required that
    //  the region fits entirely inside this file. Otherwise it will throw an
    //  exception.

    //Parameters:
    //  -   pos                 -   Starting digit position of the constant.
    //  -   (str, digits)       -   Output buffer
    //  -   (buffer, buffer_L)  -   Scratch memory
    //  -   fp_convert          -   Function pointer for binary -> char conversion.

    if (digits == 0){
        return;
    }

    //  Get boundaries
    uiL_t block_start = m_digits_per_file * m_file_id;
    uiL_t block_end   = block_start + m_digits_per_file;
    if (m_stream_end != 0 && block_end > m_digits_per_file){
        block_end = m_stream_end;
    }

    //  Check boundaries
    if (offset + digits > block_end || offset < block_start){
        std::string error = "void BasicYcdFileReader::load_digits()\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(offset);
        error += " - ";
        error += std::to_string(offset + digits);
        error += "\nAvailable Range: ";
        error += std::to_string(block_start);
        error += " - ";
        error += std::to_string(block_end);
        throw FileIO::FileException("BasicYcdFileReader::load_digits()", m_path, error);
    }

    if (Alignment::int_past_aligned<DEFAULT_ALIGNMENT>((upL_t)P) != 0){
        throw InvalidParametersException("BasicTextReader::load_digits()", "Buffer is misaligned.");
    }

    upL_t PL = Pbytes / sizeof(u64_t);
    check_BufferTooSmall("BasicYcdFileReader::load_digits()", PL, DEFAULT_ALIGNMENT / sizeof(u64_t) + 2);

    offset -= block_start;
    while (digits > 0){
        upL_t current_digits = std::min(digits, (PL - 2) * m_digits_per_word);

        load_digits_B(output, stats, offset, current_digits, (u64_t*)P, PL, parallelizer, tds);

        output += current_digits;
        offset += current_digits;
        digits -= current_digits;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
