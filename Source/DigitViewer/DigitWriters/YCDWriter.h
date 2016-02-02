/* YCDWriter.h - .ycd Writer Object
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
#ifndef _ycr_YCDWriter_H
#define _ycr_YCDWriter_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "DigitWriter.h"
#include "YCDFileWriter.h"
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
    //      (uiL_t)0 - 1    =   All digits in one file. Don't split.

    //  buffer_size:    Size of internal buffer.
    //  buffer:         Use a preallocated buffer?
    //      If you pass anything non-NULL for this parameter, it will use it as
    //      the internal buffer with size buffer_size bytes. It will not be
    //      freed upon destruction of the object.
    //  deallocator:    Deallocator. This will we called on "buffer" if it
    //      is preallocated. It will not be called if it is NULL, or if
    //      "buffer" is not preallocated.
    YCDWriter(
        std::string path,   //  UTF-8
        std::string name,   //  UTF-8
        std::string first_digits,
        ufL_t digits_per_file = (ufL_t)0 - 1,
        uiL_t start_fileid = 0,
        int radix = 10,
        upL_t buffer_size = YC_DIGITWRITER_DEFAULT_BUFFER,
        u64_t* buffer = NULL,
        void (*deallocator)(void*) = NULL
    );

    virtual void    write   (char* str, upL_t digits);

private:
    std::string path;                   //  Full path (including the id #)
    std::string name;                   //  File name

    int radix;                          //  Radix of the digits. (10 or 16)
    upL_t digits_per_word;

    std::string first_digits;           //  First digits

    ufL_t digits_per_file;              //  # of digits per compressed file

    //  File handle
    uiL_t fileid;
    YCDFileWriter file;

    //  Binary Buffer
    bool external_buffer;
    u64_t* bin_buffer;
    upL_t bin_buffer_L;
    void (*fp_free)(void*);

    void create_file(uiL_t fileid);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
