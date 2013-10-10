/* yc_TextWriter_headers.h - .txt Writer Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/25/2013
 * Last Modified    : 07/25/2013
 * 
 */

#pragma once
#ifndef _yc_TextWriter_headers_H
#define _yc_TextWriter_headers_H
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
class TextWriter : public DigitWriter{
public:
    virtual ~TextWriter();

    //  Create a new writer.
    //  If "first_digits" is not an empty string, all digits before the first
    //  decimal place will be written to the start of the file.
    //  For ASCII char input, use:   raw = false
    //  For raw integer input, use:  raw = true
    TextWriter(
        const std::wstring &path,
        const std::string &first_digits = "",
        bool raw = false,
        int radix = 10
    );

    virtual void    write   (char *str,ym_pL digits);

private:
    //  File handle
    ym_file file;

    //  Function pointer for digit conversion.
    void (*fp_convert)(char*,ym_pL);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
