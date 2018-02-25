/* BasicTextReader.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/14/2018
 * Last Modified    : 01/14/2018
 * 
 */

#pragma once
#ifndef ydv_DigitViewer_BasicTextReader_H
#define ydv_DigitViewer_BasicTextReader_H
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
class BasicTextReader : public BasicDigitReader{
public:
    BasicTextReader(const std::string& path, char radix = 0);

    virtual std::string first_digits            () override;
    virtual uiL_t       stream_end              () const override{ return m_total_digits; }
    virtual bool        range_is_available      (uiL_t offset, uiL_t digits) override;
    virtual upL_t       recommend_buffer_size   (uiL_t digits, upL_t limit) const override;

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


private:
    static const upL_t CACHE_BLOCK = 8192;

    char auto_detect_radix();

    void process(
        DigitStats* stats, char* buffer, upL_t digits
    ) const;
    void process(
        DigitStats* stats, char* buffer, upL_t digits,
        BasicParallelizer& parallelizer, upL_t tds
    ) const;

    class Action_process;


private:
    using ConvertFunction = bool (*)(char* raw_digits, const char* buffer, upL_t digits);

    std::mutex m_lock;
    FileIO::BasicFile m_file;           //  File handle

    ConvertFunction m_fp_convert;

    ufL_t m_dp_offset;                  //  Offset in the file of the first digit after the decimal place.
    ufL_t m_total_digits;               //  Digits after the decimal place.
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
