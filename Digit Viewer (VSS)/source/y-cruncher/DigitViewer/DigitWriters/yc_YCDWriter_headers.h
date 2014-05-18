/* yc_YCDWriter_headers.h - .ycd Writer Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/27/2013
 * Last Modified    : 07/27/2013
 * 
 * 
 *      The YCDWriter object needs yet another internal buffer that is
 *  independent of the digit buffer in DigitWriter.
 * 
 *  This buffer is needed to load the raw compressed data from the file. Then
 *  it is decompressed into the destination.
 *  
 */

#pragma once
#ifndef _yc_YCDWriter_headers_H
#define _yc_YCDWriter_headers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "yc_DigitWriter_headers.h"
#include "yc_YCDFileWriter_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class YCDWriter : public DigitWriter{
public:
    virtual ~YCDWriter();

    //  Create a new writer.

    //  digits_per_file:
    //      (ym_uL)0 - 1    =   All digits in one file. Don't split.

    //  buffer_size:    Size of internal buffer.
    //  buffer:         Use a preallocated buffer?
    //      If you pass anything non-NULL for this parameter, it will use it as
    //      the internal buffer with size buffer_size bytes. It will not be
    //      freed upon destruction of the object.
    //  deallocator:    Deallocator. This will we called on "buffer" if it
    //      is preallocated. It will not be called if it is NULL, or if
    //      "buffer" is not preallocated.
    YCDWriter(
        std::wstring path,
        std::wstring name,
        std::string first_digits,
        ym_uL digits_per_file = (ym_uL)0 - 1,
        ym_uL start_fileid = 0,
        int radix = 10,
        ym_pL buffer_size = YC_DIGITWRITER_DEFAULT_BUFFER,
        ym_u64 *buffer = NULL,
        void (*deallocator)(void*) = NULL
    );

    virtual void    write   (char *str,ym_pL digits);

private:
    std::wstring path;                  //  Full path (including the id #)
    std::wstring name;                  //  File name

    int radix;                          //  Radix of the digits. (10 or 16)
    ym_pL digits_per_word;

    std::string first_digits;           //  First digits

    ym_fL digits_per_file;              //  # of digits per compressed file

    //  File handle
    ym_uL fileid;
    YCDFileWriter file;

    //  Binary Buffer
    bool external_buffer;
    ym_u64 *bin_buffer;
    ym_pL bin_buffer_L;
    void (*fp_free)(void*);

    void create_file(ym_uL fileid);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
