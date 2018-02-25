/* BasicYcdFileReader.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/29/2018
 * Last Modified    : 01/29/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_BasicYcdFileReader_H
#define ydv_DigitViewer_BasicYcdFileReader_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <mutex>
#include "PublicLibs/SystemLibs/FileIO/BasicFile.h"
#include "BasicDigitReader.h"
namespace DigitViewer2{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class BasicYcdFileReader : public BasicDigitReader{
public:
    BasicYcdFileReader(const std::string& path);

    virtual std::string first_digits            () override{ return m_first_digits; }
    virtual ufL_t       stream_end              () const override{ return m_stream_end; }
    virtual bool        range_is_available      (uiL_t offset, uiL_t digits) override;
    virtual upL_t       recommend_buffer_size   (uiL_t digits, upL_t limit) const override;

    const std::string& file_version() const{ return m_file_version; }
    uiL_t   file_id     () const{ return m_file_id; }

    ufL_t   digits_per_file () const{ return m_digits_per_file; }
    upL_t   digits_per_word () const{ return m_digits_per_word; }

    virtual void load_stats(
        DigitStats& stats,
        uiL_t offset, uiL_t digits,
        void* P, upL_t Pbytes,
        BasicParallelizer& parallelizer, upL_t tds
    ) override;
    virtual void load_digits(
        char* output,
        DigitStats* stats,
        uiL_t offset, upL_t digits,
        void* P, upL_t Pbytes,
        BasicParallelizer& parallelizer, upL_t tds
    ) override;


    void print() const; //  Debugging


private:
    static const upL_t CACHE_BLOCK = 608;   //  Multiple of both 16 and 19.

    void read_words(ufL_t pos, u64_t* T, upL_t L);

    void process_blocks(
        DigitStats& stats,
        u64_t* B, upL_t BL
    ) const;
    void process_blocks(
        char* raw_digits,
        DigitStats* stats,
        u64_t* B, upL_t BL
    ) const;

    class Action_process;
    void process_blocks(
        char* raw_digits,
        DigitStats* stats,
        u64_t* B, upL_t BL,
        BasicParallelizer& parallelizer, upL_t tds
    ) const;


    //  Offsets are relative to the local file.
    void load_stats_B(
        DigitStats& stats,
        uiL_t offset, upL_t digits,
        u64_t* P, upL_t PL,
        BasicParallelizer& parallelizer, upL_t tds
    );
    void load_digits_B(
        char* output,
        DigitStats* stats,
        uiL_t offset, upL_t digits,
        u64_t* P, upL_t PL,
        BasicParallelizer& parallelizer, upL_t tds
    );


private:
    using ConvertFunction = void (*)(char*, const u64_t*, upL_t);

    std::mutex m_lock;

    std::string m_path;
    FileIO::BasicFile m_file;

    ConvertFunction m_fp_convert;

    std::string m_file_version;
    std::string m_first_digits;
    uiL_t m_stream_end;
    ufL_t m_digits_per_file;
    uiL_t m_file_id;

    upL_t m_digits_per_word;
    ufL_t m_words_in_this_file;
    ufL_t m_data_offset;        //  Offset where the actual data blocks begin.
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
