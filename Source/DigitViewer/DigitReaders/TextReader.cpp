/* yc_TextReader.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 07/28/2013
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Exception.h"
#include "DigitViewer/DigitConverter/ymb_CVN_headers.h"
#include "DigitViewer/Globals.h"
#include "TextReader.h"
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
    const std::wstring& path,
    bool raw,
    int radix_
)
    : file(path.c_str())
    , radix(radix_)
    , fp_convert(NULL)
{
    //  Auto-detect radix
    if (radix == 0)
        auto_detect_radix();
    else
        set_raw(raw);

    //  Find the decimal place
    file.set_ptr(0);
    uiL_t c = 0;
    while (1){
        char ch;
        if (file.read(&ch, 1) != 1){
            throw ym_exception("Unexpected End of File", path, FileIO::GetLastErrorCode());
        }

        c++;
        if (ch == '.'){
            dp_offset = c;
            break;
        }
        if (c == 63){
            throw ym_exception(
                "A decimal place was not found within the first 63 bytes of the file.",
                YCR_DIO_ERROR_NO_DECIMAL_PLACE
            );
        }
    }

    //  # of digits after decimal place.
    total_digits = FileIO::GetFileSize(path.c_str()) - dp_offset;
}
//TextReader::~TextReader(){
//    file.Close();
//}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TextReader::print() const{
    Console::print("File: ");
    Console::println(file.GetPath());
    Console::println();

    Console::print("Radix:                  ");
    Console::println(radix);

    Console::print("Decimal Place Offset:   ");
    Console::println(dp_offset);

    Console::print("Digits in File:         ");
    Console::println_commas(total_digits);

    Console::print("Iterator File Offset:   ");
    Console::println(iter_f_offset);

    Console::print("Iterator Buffer Offset: ");
    Console::println(iter_b_offset);
    Console::println();
}
int TextReader::get_radix() const{
    return radix;
}
ufL_t TextReader::get_digits() const{
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
                throw ym_exception("Unsupported Radix", YCR_DIO_ERROR_INVALID_BASE);
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
                throw ym_exception("Unsupported Radix", YCR_DIO_ERROR_INVALID_BASE);
        };
    }
}
bool TextReader::check_range(uiL_t start, uiL_t end){
    if (start >= end)
        throw ym_exception("Invalid Parameters", YCR_DIO_ERROR_INVALID_PARAMETERS);
    if (start >= total_digits)
        return false;
    if (end > total_digits)
        return false;
    return true;
}
std::string TextReader::get_first_digits(upL_t L){
    if (L == 0)
        return "";
    file.set_ptr(0);

    std::string str(L, '\0');
    file.read(&str[0], L);

    return str;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TextReader::read(uiL_t pos, char* str, upL_t digits){
    uiL_t end = pos + digits;

    //  Ends past the end.
    if (end > total_digits)
        throw ym_exception("Out of range.", YCR_DIO_ERROR_OUT_OF_RANGE);

    file.set_ptr(dp_offset + pos);
    if (file.read(str, digits) != digits){
        throw ym_exception("Error Reading from File", FileIO::GetLastErrorCode());
    }

    //  Convert
    if (fp_convert != NULL){
        if (fp_convert(str, digits)){
            std::string error("Invalid Digit: ");
            error += std::to_string(pos);
            error += " - ";
            error += std::to_string(pos + digits);
            throw ym_exception(std::move(error), YCR_DIO_ERROR_INVALID_DIGIT);
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
            throw ym_exception("Unsupported Radix", YCR_DIO_ERROR_INVALID_BASE);
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
    file.set_ptr(0);

    for (upL_t c = 0; c < 64; c++){
        char ch;
        if (file.read(&ch, 1) != 1){
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
