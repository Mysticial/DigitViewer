/* BasicYcdFileWriter.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/04/2018
 * Last Modified    : 02/04/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_BasicYcdFileWriter_H
#define ydv_DigitViewer_BasicYcdFileWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <mutex>
#include "PublicLibs/BasicLibs/SparseRegion.h"
#include "PublicLibs/SystemLibs/FileIO/BasicFile.h"
#include "BasicDigitWriter.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicYcdFileWriter : public BasicDigitWriter{
public:
    BasicYcdFileWriter(
        const std::string& path,    //  UTF-8
        const std::string& first_digits,
        char radix,
        ufL_t digits_per_file, uiL_t stream_end,
        uiL_t fileid
    );
    ~BasicYcdFileWriter();

    virtual upL_t recommend_buffer_size(uiL_t digits, upL_t limit) const override;
    virtual std::unique_ptr<BasicDigitReader> close_and_get_basic_reader() override;

    uiL_t file_id() const{ return m_file_id; }
    const std::string& path() const{ return m_path; }

    virtual void store_digits(
        const char* input,
        uiL_t offset, upL_t digits,
        void* P, upL_t Pbytes,
        BasicParallelizer& parallelizer, upL_t tds
    ) override;


    void print() const; //  Debugging


private:
    void read_words(ufL_t pos, u64_t* T, upL_t L);
    void store_words(ufL_t pos, const u64_t* T, upL_t L);
    void store_digits_B(
        const char* input,
        ufL_t offset, upL_t digits,
        u64_t* P, upL_t Pbytes,
        BasicParallelizer& parallelizer, upL_t tds
    );


private:
    using ConvertForward = bool (*)(u64_t*, const char*, upL_t);
    using ConvertInverse = void (*)(char*, const u64_t*, upL_t);

    std::mutex m_lock;

    std::string m_path;
    FileIO::BasicFile m_file;

    uiL_t m_stream_end;
    ufL_t m_digits_per_file;
    uiL_t m_file_id;

    ConvertForward m_fp_convert_forward;
    ConvertInverse m_fp_convert_inverse;

    //  File offset of where the "stream_end" field is.
    //  This is needed because this value isn't determined until the file is
    //  closed. That's when the value is written. So we need to know where to
    //  seek back to. This is also the reason why this is the only zero-padded
    //  field in the compressed digit format.
    ufL_t m_offset_stream_end;

    upL_t m_digits_per_word;
    ufL_t m_words_in_this_file;
    ufL_t m_data_offset;        //  Offset where the actual data blocks begin.

    //  Keep track of which parts of the file have been written to.
    Region<uiL_t> m_target;
    SparseRegion<uiL_t> m_written;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
