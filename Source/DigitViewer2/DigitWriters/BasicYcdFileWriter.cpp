/* BasicYcdFileWriter.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/04/2018
 * Last Modified    : 02/09/2018
 * 
 *      Implements a single file of a set of .ycd compressed files.
 * 
 *  When the file is created, it will have "(incomplete)" appended to its name
 *  to indicate that it has not been completely written out.
 * 
 *  Once the file is completely written out, it is automatically closed and
 *  renamed to the proper name. At this point, no more writes will be accepted.
 * 
 *  This behavior of disallowing further writes is just a side-effect of the
 *  "FileIO::BasicFile" class. This restriction may be removed in the future,
 *  though there are no real use-cases of it.
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <algorithm>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/Exceptions/BufferTooSmallThrower.h"
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "PublicLibs/SystemLibs/FileIO/FileException.h"
#include "PublicLibs/SystemLibs/FileIO/FileIO.h"
#include "DigitViewer/Globals.h"
#include "DigitViewer2/RawToCompressed/RawToCompressed.h"
#include "BasicYcdFileWriter.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BasicYcdFileWriter::BasicYcdFileWriter(
    const std::string& path,
    const std::string& first_digits,
    char radix,
    ufL_t digits_per_file, uiL_t stream_end,
    uiL_t fileid
)
    : m_path(path + ".ycd")
    , m_file(0, path + " (incomplete).ycd")
    , m_stream_end(stream_end)
    , m_digits_per_file(digits_per_file)
    , m_file_id(fileid)
    , m_target(
        fileid * digits_per_file,
        stream_end == 0
            ? (fileid + 1) * digits_per_file
            : std::min((fileid + 1) * digits_per_file, stream_end)
    )
{
    using namespace DigitViewer;

    m_radix = radix;

    uiL_t s = m_digits_per_file * m_file_id;
    if (stream_end != 0 && stream_end <= s){
        throw InvalidParametersException(
            "BasicYcdFileWriter::BasicYcdFileWriter()",
            "File ID is out of range."
        );
    }

    //  Make the header
    std::string header;
    header += "#Compressed Digit File\r\n\r\n";

    header += YCF_CDF_TOKEN_FileVersion;
    header += '\t';
    header += YCF_CDF_FileVersion;
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_Base;
    header += '\t';
    header += std::to_string(radix);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_FirstDigits;
    header += '\t';
    header += first_digits;
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_TotalDigits;
    header += '\t';
    m_offset_stream_end = header.size();
//    header += to_string_max(stream_end);
    header += std::to_string(stream_end);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_BlockSize;
    header += '\t';
    header += std::to_string(digits_per_file);
    header += "\r\n";

    header += YCF_CDF_TOKEN_BlockID;
    header += '\t';
    header += std::to_string(m_file_id);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_EndHeader;
    header += "\r\n\r\n";
    header += '\0';

    //  Write the header
    upL_t size = header.size();
    if (m_file.write(&header[0], size) != size){
        FileIO::PrintLastError();
        throw FileIO::FileException(
            "YCDFileWriter::YCDFileWriter()",
            path,
            "Error writing to file."
        );
    }
    m_data_offset = size;

    switch (radix){
        case 10:
            m_digits_per_word = 19;
            m_words_in_this_file = (digits_per_file - 1) / 19 + 1;
            m_fp_convert_forward = RawToCompressed::dec_to_i64;
            m_fp_convert_inverse = RawToCompressed::i64_to_dec;
            break;
        case 16:
            m_digits_per_word = 16;
            m_words_in_this_file = (digits_per_file - 1) / 16 + 1;
            m_fp_convert_forward = RawToCompressed::hex_to_i64;
            m_fp_convert_inverse = RawToCompressed::i64_to_hex;
            break;
        default:
            throw FileIO::FileException("BasicYcdFileWriter::BasicYcdFileWriter()", path, "Unsupported Radix");
    }
}
BasicYcdFileWriter::~BasicYcdFileWriter(){
    if (!(m_written == m_target)){
        Console::println_labelc("Warning, closing incomplete file", m_path);
        m_written.print();
    }
}
upL_t BasicYcdFileWriter::recommend_buffer_size(uiL_t digits, upL_t limit) const{
    if (digits == 0){
        return 0;
    }
    uiL_t words = (digits - 1) / m_digits_per_word + 1;
    upL_t bytes = (upL_t)std::min((words + 2) * sizeof(u64_t), (uiL_t)limit);
    bytes = std::max(bytes, DEFAULT_ALIGNMENT + 2 * sizeof(u64_t));
    return bytes;
}
std::unique_ptr<BasicDigitReader> BasicYcdFileWriter::close_and_get_basic_reader(){
    //  Not supported.
    return nullptr;
}
void BasicYcdFileWriter::print() const{
    Console::println(m_path);
    Console::println();

//    Console::println_labelm("file_version:", m_file_version);
    Console::println_labelm("radix:", m_radix);
//    Console::println_labelm("first_digits:", m_first_digits);
    Console::println_labelm_commas("stream_end:", m_stream_end);
    Console::println_labelm_commas("digits_per_file:", m_digits_per_file);
    Console::println_labelm_commas("file_id:", m_file_id);
    Console::println();

    Console::println_labelm_commas("digits_per_word:", m_digits_per_word);
    Console::println_labelm_commas("words_in_this_file:", m_words_in_this_file);
    Console::println_labelm_commas("data_offset:", m_data_offset);
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicYcdFileWriter::read_words(ufL_t pos, u64_t* T, upL_t L){
    if (pos + L > m_words_in_this_file){
        Console::Warning("Internal Error: Read out of Bounds");
        std::string error = "BasicYcdFileWriter::read_word()\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(pos);
        error += " - ";
        error += std::to_string(pos + L);
        error += "\nAvailable Range: ";
        error += std::to_string(0);
        error += " - ";
        error += std::to_string(m_words_in_this_file);
        throw FileIO::FileException("BasicYcdFileWriter::read_words()", m_path, error);
    }

//    std::lock_guard<std::mutex> lg(m_lock);

    //  Set file pointer
    m_file.set_ptr(m_data_offset + pos * sizeof(u64_t));

    //  Read
    upL_t words_read = m_file.read(T, L * sizeof(u64_t)) / sizeof(u64_t);
    if (words_read != L){
        memset(T + words_read, 0, (L - words_read) * sizeof(u64_t));
    }

    //  If the word is invalid, it means it hasn't been written to yet. So zero it.
    if (m_radix != 16){
        for (upL_t c = 0; c < L; c++){
            if (T[c] >= 10000000000000000000ull){
                T[c] = 0;
            }
        }
    }
}
void BasicYcdFileWriter::store_words(ufL_t pos, const u64_t* T, upL_t L){
    if (pos + L > m_words_in_this_file){
        Console::Warning("Internal Error: Write out of Bounds");
        std::string error = "BasicYcdFileWriter::store_words()\n";
        error += "Store out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(pos);
        error += " - ";
        error += std::to_string(pos + L);
        error += "\nAvailable Range: ";
        error += std::to_string(0);
        error += " - ";
        error += std::to_string(m_words_in_this_file);
        throw FileIO::FileException("BasicYcdFileWriter::store_words()", m_path, error);
    }

//    std::lock_guard<std::mutex> lg(m_lock);

    //  Set file pointer
    m_file.set_ptr(m_data_offset + pos * sizeof(u64_t));

    //  Read
    upL_t words_written = m_file.write(T, L * sizeof(u64_t)) / sizeof(u64_t);
    if (words_written != L){
        throw FileIO::FileException("BasicYcdFileReader::store_words()", m_path, "Error Writing to File");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicYcdFileWriter::store_digits_B(
    const char* input,
    ufL_t offset, upL_t digits,
    u64_t* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    //  Conditions:
    //   -  digits <= (PL - 2) * m_digits_per_word

    if (digits == 0){
        return;
    }

    ufL_t end = offset + digits;
    ufL_t word_s = offset / m_digits_per_word;
    ufL_t word_e = end / m_digits_per_word;

    ufL_t Wstart = word_s;
    u64_t* Pstart = P;
    upL_t digits_left = digits;

    std::lock_guard<std::mutex> lg(m_lock);

    //  Start Filter
    ufL_t read_s = word_s * m_digits_per_word;
    if (read_s != offset){
//        cout << "Start Filter" << endl;

        //  Load and filter
        char buffer[19];
        read_words(word_s, P, 1);
        m_fp_convert_inverse(buffer, P, 1);

        //  Compute sizes
        upL_t diff = (upL_t)(offset - read_s);
        upL_t size = m_digits_per_word - diff;

        //  Make sure we don't overrun the end.
        size = std::min(size, digits_left);

        //  Copy to output.
        memcpy(buffer + diff, input, size);

        //  Store
        if (m_fp_convert_forward(P, buffer, 1)){
            throw InvalidParametersException("BasicYcdFileWriter::store_digits_B()", "Invalid Digit");
        }

        input += size;
        digits_left -= size;
        word_s++;
        P++;
    }

    if (digits_left == 0){
        store_words(Wstart, Pstart, P - Pstart);
        return;
    }

    //  Steady State
    upL_t blocks = (upL_t)(word_e - word_s);
    if (blocks > 0){
//        cout << "Steady" << endl;
//        m_fp_convert_forward(input, P, blocks); //  TODO: Parallelize
        bool bad = RawToCompressed::raw_to_i64(
            m_fp_convert_forward, m_digits_per_word,
            P, input, blocks,
            parallelizer, tds
        );
        if (bad){
            throw InvalidParametersException("BasicYcdFileWriter::store_digits_B()", "Invalid Digit");
        }

        upL_t current_digits = blocks * m_digits_per_word;
        P += blocks;
        input += current_digits;
        digits_left -= current_digits;
        word_s += blocks;
    }

    //  End Filter
    if (word_e * m_digits_per_word < end){
//        cout << "End Filter" << endl;
        char buffer[19];
        read_words(word_s, P, 1);
        m_fp_convert_inverse(buffer, P, 1);

        memcpy(buffer, input, digits_left);
        if(m_fp_convert_forward(P, buffer, 1)){
            throw InvalidParametersException("BasicYcdFileWriter::store_digits_B()", "Invalid Digit");
        }
        P++;
    }

    //  Store to file.
    store_words(Wstart, Pstart, P - Pstart);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BasicYcdFileWriter::store_digits(
    const char* input,
    uiL_t offset, upL_t digits,
    void* P, upL_t Pbytes,
    BasicParallelizer& parallelizer, upL_t tds
){
    if (!m_file.is_open()){
        throw InvalidParametersException("BasicYcdFileWriter::store_digits()", "File is closed.");
    }

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
        std::string error = "void BasicYcdFileWriter::store_digits()\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(offset);
        error += " - ";
        error += std::to_string(offset + digits);
        error += "\nAvailable Range: ";
        error += std::to_string(block_start);
        error += " - ";
        error += std::to_string(block_end);
        throw FileIO::FileException("BasicYcdFileWriter::store_digits()", m_path, error);
    }

    upL_t PL = Pbytes / sizeof(u64_t);
    check_BufferTooSmall("BasicYcdFileWriter::store_digits()", PL, DEFAULT_ALIGNMENT / sizeof(u64_t) + 2);

    uiL_t s = offset;
    uiL_t e = offset + digits;

    //  Break it up into blocks that fit in the buffer.
    offset -= block_start;
    while (digits > 0){
        upL_t current_digits = std::min(digits, (PL - 2) * m_digits_per_word);

        store_digits_B(input, (ufL_t)offset, current_digits, (u64_t*)P, PL, parallelizer, tds);

        input += current_digits;
        offset += current_digits;
        digits -= current_digits;
    }

    //  Update the written-to tracker.
    std::lock_guard<std::mutex> lg(m_lock);
    m_written |= Region<ufL_t>(s, e);

    //  If the file is complete, close it.
    if (m_written == m_target){
        std::string path = m_file.GetPath();
        m_file.close();
        FileIO::RenameFile(path, m_path);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
