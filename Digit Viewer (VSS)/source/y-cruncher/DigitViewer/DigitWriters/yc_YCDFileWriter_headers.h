/* yc_YCDFileWriter_headers.h - .ycd Writer Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/28/2013
 * Last Modified    : 07/29/2013
 * 
 * 
 *  This class represents a file handle to a single .ycd file. When constructed,
 *  it creates a .ycd, writes the header, and handles all digit writes to it.
 * 
 *  Note that this writer class only accepts raw input. There is no option to
 *  let it accept plain-text input. This is because the "CVN" module does not
 *  have support for plain-text -> compressed conversions.
 * 
 */

#pragma once
#ifndef _yc_YCDFileWriter_headers_H
#define _yc_YCDFileWriter_headers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "yc_DigitWriter_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YC_DIGITWRITER_MAX_DIGIT_WIDTH     20
class YCDFileWriter{
public:
    //  Movable
    YCDFileWriter();
    YCDFileWriter(YCDFileWriter &&);
    void operator=(YCDFileWriter &&);
    ~YCDFileWriter();
private:
    //  But not copyable
    YCDFileWriter(const YCDFileWriter &);
    void operator=(const YCDFileWriter &);

public:

    //  Create a new writer
    YCDFileWriter(
        std::wstring path,
        const std::string &first_digits,
        ym_uL digits_per_file,
        ym_uL fileid = 0,
        int radix = 10
    );

    void close();

    ym_pL write_chars(
        const char *str,ym_pL digits,
        ym_u64 *buffer,ym_pL buffer_L
    );

    bool isValid() const;

private:
    std::wstring path;
    ym_file file;

    int radix;                          //  Radix of the digits. (10 or 16)
    ym_pL digits_per_word;
    void (*fp_convert)(ym_u64*,const char*,ym_pL);

    ym_fL words_per_file;
    ym_fL digits_per_file;
    ym_uL file_id;

    //  File offset of where the "total_digits" field is.
    //  This is needed because this value isn't determined until the file is
    //  closed. That's when the value is written. So we need to know where to
    //  seek back to. This is also the reason why this is the only zero-padded
    //  field in the compressed digit format.
    ym_uL offset_total_digits;

    ym_fL pos_word;         //  Current words written (and flushed) to the file.
    ym_fL pos_char;         //  Current digits written to the file. (including unflushed digits)
    char str_buffer[20];    //  Buffer to handle end points.
    ym_pL buffered;         //  # of digits in the buffer.

    void write_words(ym_u64 *T,ym_pL L);
    void flush();

};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
