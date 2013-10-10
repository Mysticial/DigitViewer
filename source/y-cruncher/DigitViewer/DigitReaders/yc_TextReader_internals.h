/* yc_TextReader_internals.h - .txt Reader Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 07/28/2013
 * 
 */

#pragma once
#ifndef _yc_TextReader_internals_H
#define _yc_TextReader_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "yc_TextReader_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace DigitViewer{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
TextReader::TextReader(
    const std::wstring &path,
    bool raw,
    int radix_
)
    : radix(radix_)
    , fp_convert(NULL)
{

    //  Open the file
    if (ym_file_open_r(NULL,&file,path.c_str()))
        throw ym_exception("Unable to open file.",path,ym_file_get_error_code());

    //  Auto-detect radix
    if (radix == 0)
        auto_detect_radix();
    else
        set_raw(raw);

    //  Find the decimal place
    ym_file_setptr(&file,0);
    ym_uL c = 0;
    while (1){
        char ch;
        if (ym_file_rd(&file,&ch,1) != 1){
            ym_file_close(&file);
            throw ym_exception("Unexpected End of File",path,ym_file_get_error_code());
        }

        c++;
        if (ch == '.'){
            dp_offset = c;
            break;
        }
        if (c == 63){
            ym_file_close(&file);
            throw ym_exception(
                "A decimal place was not found within the first 63 bytes of the file.",
                YCR_DIO_ERROR_NO_DECIMAL_PLACE
            );
        }
    }

    //  # of digits after decimal place.
    total_digits = ym_file_getsize(path.c_str()) - dp_offset;
}
TextReader::~TextReader(){
    ym_file_close(&file);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TextReader::print() const{
    ymo_print_astr("File: ");
    ymo_print_wstr(file.path);
    ymo_print_astr("\n\n");

    ymo_print_astr("Radix:                  ");
    ymo_print_uL(radix);
    ymo_print_astr("\n");

    ymo_print_astr("Decimal Place Offset:   ");
    ymo_print_uL(dp_offset);
    ymo_print_astr("\n");

    ymo_print_astr("Digits in File:         ");
    ymo_print_uL(total_digits);
    ymo_print_astr("\n\n");

    ymo_print_astr("Iterator File Offset:   ");
    ymo_print_uL(iter_f_offset);
    ymo_print_astr("\n");

    ymo_print_astr("Iterator Buffer Offset: ");
    ymo_print_uL(iter_b_offset);
    ymo_print_astr("\n\n");
}
int TextReader::get_radix() const{
    return radix;
}
ym_fL TextReader::get_digits() const{
    return total_digits;
}
void TextReader::set_raw(bool raw){
    //  Clear the cache
    iter_b_offset = buffer_L;
    
    if (raw){
        //  User wants output to be raw.
        switch (radix){
            case 10:
                fp_convert = ymb_CVN_strd_to_rawd_f;
                break;
            case 16:
                fp_convert = ymb_CVN_strh_to_rawh_f;
                break;
            default:
                throw ym_exception("Unsupported Radix",YCR_DIO_ERROR_INVALID_BASE);
        };
    }else{
        //  User wants output to be text.
        switch (radix){
            case 10:
                fp_convert = NULL;
                break;
            case 16:
                fp_convert = NULL;
                break;
            default:
                throw ym_exception("Unsupported Radix",YCR_DIO_ERROR_INVALID_BASE);
        };
    }
}
bool TextReader::check_range(ym_uL start,ym_uL end){
    if (start >= end)
        throw ym_exception("Invalid Parameters",YCR_DIO_ERROR_INVALID_PARAMETERS);
    if (start >= total_digits)
        return false;
    if (end > total_digits)
        return false;
    return true;
}
std::string TextReader::get_first_digits(ym_pL L){
    if (L == 0)
        return "";
    ym_file_setptr(&file,0);

    std::string str(L,'\0');
    ym_file_rd(&file,&str[0],L);

    return str;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TextReader::read(ym_uL pos,char *str,ym_pL digits){
    ym_uL end = pos + digits;

    //  Ends past the end.
    if (end > total_digits)
        throw ym_exception("Out of range.",YCR_DIO_ERROR_OUT_OF_RANGE);

    ym_file_setptr(&file,dp_offset + pos);
    if (ym_file_rd(&file,str,digits) != digits){
        throw ym_exception("Error Reading from File",ym_file_get_error_code());
    }

    //  Convert
    if (fp_convert != NULL){
        if (fp_convert(str,digits)){
            std::string error("Invalid Digit: ");
            error += std::to_string(pos);
            error += " - ";
            error += std::to_string(pos + digits);
            throw ym_exception(std::move(error),YCR_DIO_ERROR_INVALID_DIGIT);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TextReader::set_radix(int radix){
    //  Make sure the radix is valid before setting it. Don't let an exception
    //  leave the object in an invalid state.
    switch (radix){
        case 10:
        case 16:
            break;
        default:
            throw ym_exception("Unsupported Radix",YCR_DIO_ERROR_INVALID_BASE);
    }
    this->radix = radix;
    if (fp_convert != NULL)
        set_raw(true);
}
void TextReader::auto_detect_radix(){
    //  Clear the cache
    iter_b_offset = buffer_L;

    //  Assume radix 10 until a hexadecimal digit is found.
    int radix = 10;

    //  Read the first 64 bytes to guess the radix.
    ym_file_setptr(&file,0);

    for (ym_pL c = 0; c < 64; c++){
        char ch;
        if (ym_file_rd(&file,&ch,1) != 1){
            break;
        }

        //  Skip the decimal place.
        if (ch == '.')
            continue;

        //  Decimal digit
        if ('0' <= ch && ch <= '9')
            continue;

        //  Hexadecimal digit
        if ('a' <= ch && ch <= 'f'){
            radix = 16;
            break;
        }

        throw ym_exception("Invalid Digit");
    }

    set_radix(radix);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
