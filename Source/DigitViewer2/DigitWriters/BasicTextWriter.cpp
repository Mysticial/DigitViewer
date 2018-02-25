/* BasicTextWriter.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/03/2018
 * Last Modified    : 02/03/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/BasicLibs/Alignment/DefaultAlignment.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "PublicLibs/SystemLibs/Concurrency/Parallelizers.h"
#include "DigitViewer2/RawToAscii/RawToAscii.h"
#include "DigitViewer2/DigitReaders/BasicTextReader.h"
#include "BasicTextWriter.h"
namespace DigitViewer2{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicTextWriter::BasicTextWriter(
    const std::string& path,
    const std::string& first_digits,
    char radix
)
    : m_file(0, path)
    , m_dp_offset(0)
{
    m_radix = radix;
    switch (radix){
    case 10:
        m_fp_convert = RawToAscii::raw_to_dec;
        break;
    case 16:
        m_fp_convert = RawToAscii::raw_to_hex;
        break;
    default:
        throw InvalidParametersException("BasicTextWriter::BasicTextWriter()", "Invalid radix.");
    }

    //  Write the first digits.
    if (first_digits.size() != 0){
        upL_t decimal_offset = first_digits.find('.');
        if (decimal_offset == std::string::npos){
            throw FileIO::FileException("TextWriter::TextWriter()", path, "No decimal place was found.");
        }
        m_file.write(first_digits.c_str(), decimal_offset + 1);
        m_dp_offset = decimal_offset + 1;
    }
}
upL_t BasicTextWriter::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    upL_t bytes = (upL_t)std::min(digits, (uiL_t)limit);
    bytes = std::max(bytes, DEFAULT_ALIGNMENT);
    return bytes;
}
std::unique_ptr<BasicDigitReader> BasicTextWriter::close_and_get_basic_reader(){
    std::string path = m_file.GetPath();
    m_file.close();
    return std::make_unique<BasicTextReader>(path, m_radix);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicTextWriter::store_digits(
    const char* input,
    uiL_t offset, upL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (!m_file.is_open()){
        throw InvalidParametersException("BasicTextWriter::store_digits()", "File is closed.");
    }

    //  Break it up into blocks that fit in the buffer.
    char* buffer = (char*)P;
    while (digits > 0){
        upL_t current_digits = std::min(Pbytes, digits);

        bool bad = RawToAscii::parallel_convert(
            m_fp_convert,
            buffer, input, current_digits,
            parallelizer, tds
        );
        if (bad){
            throw InvalidParametersException("BasicTextWriter::store_digits()", "Invalid Digit");
        }

        std::lock_guard<std::mutex> lg(m_lock);
        m_file.set_ptr(m_dp_offset + (ufL_t)offset);
        m_file.write(buffer, current_digits);

        input += current_digits;
        offset += current_digits;
        digits -= current_digits;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
