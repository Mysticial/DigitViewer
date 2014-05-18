/* yc_YCDFileWriter_internals.h - .ycd Writer Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/28/2013
 * Last Modified    : 07/29/2013
 * 
 */

#pragma once
#ifndef _yc_YCDFileWriter_internals_H
#define _yc_YCDFileWriter_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
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
//  Helpers
void write_uL_max(char *str,ym_uL x){
    ym_pL c = YC_DIGITWRITER_MAX_DIGIT_WIDTH;
    while (x != 0){
        str[--c] = (char)(x % 10) + '0';
        x /= 10;
    }
    while (c > 0){
        str[--c] = '0';
    }
}
std::string to_string_max(ym_uL x){
    char str[YC_DIGITWRITER_MAX_DIGIT_WIDTH + 1];
    str[YC_DIGITWRITER_MAX_DIGIT_WIDTH] = '\0';
    write_uL_max(str,x);
    return std::string(str);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
YCDFileWriter::YCDFileWriter()
    : buffered(0)
{
    file.path = NULL;
}
void YCDFileWriter::operator=(YCDFileWriter &&x){
    close();

    path                = std::move(x.path);
    file                = x.file;
    radix               = x.radix;
    digits_per_word     = x.digits_per_word;
    fp_convert          = x.fp_convert;
    words_per_file      = x.words_per_file;
    digits_per_file     = x.digits_per_file;
    file_id             = x.file_id;
    offset_total_digits = x.offset_total_digits;
    pos_word            = x.pos_word;
    pos_char            = x.pos_char;
    buffered            = x.buffered;
    memcpy(str_buffer,x.str_buffer,sizeof(str_buffer));

    x.file.path = NULL;
}
YCDFileWriter::~YCDFileWriter(){
    close();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
YCDFileWriter::YCDFileWriter(
    std::wstring path_,
    const std::string &first_digits,
    ym_uL digits_per_file_,
    ym_uL fileid_,
    int radix_
)
    : path(std::move(path_))
    , radix(radix_)
    , digits_per_file(digits_per_file_)
    , file_id(fileid_)
    , pos_word(0)
    , pos_char(0)
    , buffered(0)
{
    //  Make the header
    std::string header;
    header += "#Compressed Digit File\r\n\r\n";

    header += YCF_CDF_TOKEN_FileVersion;
    header += '\t';
    header += YCF_CDF_FileVersion;
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_Base;
    header += '\t';
    header += std::to_string(radix);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_FirstDigits;
    header += '\t';
    header += first_digits;
    header += "\r\n\r\n";
    
    header += YCF_CDF_TOKEN_TotalDigits;
    header += '\t';
    offset_total_digits = header.size();
    header += to_string_max(0);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_BlockSize;
    header += '\t';
    header += std::to_string(digits_per_file);
    header += "\r\n";

    header += YCF_CDF_TOKEN_BlockID;
    header += '\t';
    header += std::to_string(file_id);
    header += "\r\n\r\n";

    header += YCF_CDF_TOKEN_EndHeader;
    header += "\r\n\r\n";
    header += '\0';

    //  Create the file
    ym_file_create(NULL,&file,0,path.c_str());

    //  Write the header
    ym_pL size = header.size();
    if (ym_file_wr(&file,&header[0],size) != size){
        throw ym_exception(
            "Error writing to file.",
            std::move(path_),
            ym_file_get_error_code()
        );
    }

    //  Radix-specific things
    switch (radix){
        case 10:
            digits_per_word = 19;
            fp_convert = ymb_CVN_rawd_to_u64d_f;
            break;
        case 16:
            digits_per_word = 16;
            fp_convert = ymb_CVN_rawh_to_u64b_f;
            break;
        default:
            throw ym_exception("Unsupported Radix",YCR_DIO_ERROR_INVALID_BASE);
    }

    words_per_file = (digits_per_file - 1) / digits_per_word + 1;
    memset(str_buffer,0,sizeof(str_buffer));
}
void YCDFileWriter::close(){
    //  The object isn't valid anyway.
    if (file.path == NULL)
        return;

    //  Flush buffer
    flush();
    
    //  If the file isn't full, then it is the end-file.
    if (pos_char != digits_per_file){
        //  Set the total_digits field.
        ym_file_setptr(&file,offset_total_digits);
        std::string digit_string = to_string_max(digits_per_file * file_id + pos_char);
        if (ym_file_wr(&file,digit_string.c_str(),
            YC_DIGITWRITER_MAX_DIGIT_WIDTH) != YC_DIGITWRITER_MAX_DIGIT_WIDTH
        ){
            //  Yes, this is dangerous if this is called from a destructor.
            //  Normally you can't recover from it anyway, but you can always
            //  call this function before you destruct.
            throw ym_exception(
                "Error writing to file.",
                path,
                ym_file_get_error_code()
            );
        }
    }

    //  Close the file
    ym_file_close(&file);
    file.path = NULL;
}
bool YCDFileWriter::isValid() const{
    return file.path != NULL;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDFileWriter::write_words(ym_u64 *T,ym_pL L){
    if (pos_word + L > words_per_file)
        throw ym_exception("Attempted to write beyond the range of this file.",YCR_DIO_ERROR_INTERNAL);

    ym_pL bytes = L * sizeof(ym_u64);
    if (ym_file_wr(&file,T,bytes) != bytes){
        throw ym_exception(
            "Error writing to file.",
            path,
            ym_file_get_error_code()
        );
    }
}
ym_pL YCDFileWriter::write_chars(
    const char *str,ym_pL digits,
    ym_u64 *buffer,ym_pL buffer_L
){
    //  Write digits to the file.
    //  This method returns the number of digits that are actually written.
    //  If the return value is less than "digits", this implies that the end of
    //  the file has been reached. So the caller must continue by creating the
    //  next file and calling with (str + return value) and (digits - return value).

    //  If the end of the file is reached, the file is closed.

    if (file.path == NULL)
        throw ym_exception("This file is already closed.",YCR_DIO_ERROR_INTERNAL);

    ym_pL start_digits = digits;

    while (digits > 0){
        //  The file is full
        if (pos_char == digits_per_file){
            close();
            break;
        }

        //  Under some conditions, we must push 1 digit at a time.
        //  Entering this statement will push exactly 1 digit and continue the loop.
        ym_uL file_left = digits_per_file - pos_char;
        if (buffered != 0 ||                //  Buffer is not empty
            digits < digits_per_word ||     //  Digits is less then the word-size
            file_left < digits_per_word     //  The remainder of the file is less than the word-size
        ){
            //  Push the digit
            str_buffer[buffered++] = *str++;
            digits--;
            pos_char++;

            //  Buffer is full. Flush it.
            if (buffered == digits_per_word){
                ym_u64 tmp;
                fp_convert(&tmp,str_buffer,1);
                write_words(&tmp,1);
                buffered = 0;
            }
            continue;
        }

        //  At this point, the buffer is empty. This means that it is aligned
        //  to the word. Furthermore, there is more than one word to process.

        //  This makes it possible to run the fast converter.

        //  Convert this many digits this iteration.
        ym_pL current_digits = digits;

        //  Don't overrun the file.
        if (current_digits > file_left)
            current_digits = (ym_pL)file_left;

        //  Get # of words.
        ym_pL words = current_digits / digits_per_word;

        //  Don't overrun the buffer.
        if (words > buffer_L)
            words = buffer_L;

        current_digits = words * digits_per_word;

        //  Convert
        fp_convert(buffer,str,words);
        write_words(buffer,words);

        str      += current_digits;
        digits   -= current_digits;
        pos_char += current_digits;
    }

    return start_digits - digits;
}
void YCDFileWriter::flush(){
    //  Should only be called at the end of a file.

    //  Nothing buffered
    if (buffered == 0)
        return;

    //  Write zeros to rest of buffer
    while (buffered < digits_per_word)
        str_buffer[buffered++] = 0;

    //  Flush
    ym_u64 tmp;
    fp_convert(&tmp,str_buffer,1);
    write_words(&tmp,1);
    buffered = 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
