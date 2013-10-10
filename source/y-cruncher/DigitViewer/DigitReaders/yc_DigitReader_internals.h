/* yc_DigitReader_internals.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/22/2013
 * Last Modified    : 07/28/2013
 * 
 */

#pragma once
#ifndef _yc_DigitReader_internals_H
#define _yc_DigitReader_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "yc_DigitReader_headers.h"
#include "yc_TextReader_internals.h"
#include "yc_YCDFileReader_internals.h"
#include "yc_YCDReader_internals.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DigitReader::DigitReader()
    : buffer_L(0)
    , iter_f_offset(0)
    , iter_b_offset(0)
{
}
void DigitReader::clear_buffer(){
    //  To clear the buffer, set the offset to the size of the buffer.
    //  This will trick the next digit load into thinking that all the digits
    //  in the buffer have been used up. Thereby, forcing it to reload the
    //  buffer.
    iter_b_offset = buffer_L;
}
void DigitReader::set_pos(ym_uL offset){
    ym_uL total_digits = get_digits();
    if (total_digits != 0 && offset > total_digits){
        throw ym_exception("Out of range.",YCR_DIO_ERROR_OUT_OF_RANGE);
    }

    //  Set the offset
    iter_f_offset = offset;

    //  Setting the position will invalidate the buffer.
    clear_buffer();
}
void DigitReader::read(char *str,ym_pL digits){
    while (digits > 0){
        //  Buffer is empty
        if (iter_b_offset == buffer_L){
            reload();
        }

        ym_pL block = buffer_L - iter_b_offset;
        if (block > digits)
            block = digits;

        memcpy(str,&buffer[0] + iter_b_offset,block);
        
        iter_f_offset += block;
        iter_b_offset += block;
        str += block;
        digits -= block;
    }
}
YM_NOINLINE void DigitReader::reload(){
    //  Reloads the buffer.

    //  Regardless of whether this method throws, it will leave the buffer in
    //  a consistent state.

    //  Buffer isn't initialized yet.
    if (buffer_L == 0){
        ym_pL buffer_size = YC_DIGITREADER_DEFAULT_BUFFER;
        buffer = std::unique_ptr<char[]>(new char[buffer_size]);

        //  Do this assignment last - just in case the above throws.
        buffer_L = buffer_size;
    }

    //  Wipe the buffer now. This will keep the object in a consistent state
    //  in the event of an exception.
    clear_buffer();

    ym_uL total_digits = get_digits();
    if (total_digits == 0){
        throw ym_exception(
            "DigitReader::reload() does not support unknown sizes.\n"
            "You must override this function when total_digits could be 0.",
            YCR_DIO_ERROR_INTERNAL
        );
    }

    ym_pL read  = buffer_L;
    ym_uL start = iter_f_offset;
    ym_uL end   = iter_f_offset + read;
    if (start >= total_digits){
        std::string error("No more digits left: ");
        error += std::to_string(total_digits);
        throw ym_exception(std::move(error),YCR_DIO_ERROR_NO_DIGITS_LEFT);
    }

    //  Near the end. Read the rest and fill only part of the buffer.
    ym_pL current_b_offset = 0;
    if (end > total_digits){
        read = (ym_pL)(total_digits - start);
        current_b_offset = buffer_L - read;
    }

    //  Read into buffer
    this->read(start,&buffer[current_b_offset],read);

    //  Do this assignment last to protect against an exception.
    iter_b_offset = current_b_offset;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Factory
std::unique_ptr<DigitReader> OpenDigitFile(std::wstring path,bool raw,ym_pL buffer_size){
    //  Extract the extension
    size_t extension_offset = path.rfind('.');
    if (extension_offset >= path.size()){
        throw ym_exception("No Extension found.",std::move(path));
    }
    std::wstring extension = path.substr(extension_offset);

    if (extension == L".txt"){
        return std::unique_ptr<DigitReader>(new TextReader(path,raw,0));
    }else if (extension == L".ycd"){
        return std::unique_ptr<DigitReader>(new YCDReader(std::move(path),raw,buffer_size));
    }else{
        throw ym_exception("Unrecognized Extension",std::move(extension));
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
