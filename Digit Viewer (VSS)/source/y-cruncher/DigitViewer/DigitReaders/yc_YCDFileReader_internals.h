/* yc_YCDFile_internals.h - .ycd Reader Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 07/28/2013
 * 
 */

#pragma once
#ifndef _yc_YCDFile_internals_H
#define _yc_YCDFile_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "yc_YCDFileReader_headers.h"
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
std::string grab_until_delim(ym_file *file,char delim){
    //  Streams characters from "file" into a string until a deliminator is found.

    std::string out;

    char ch;
    do{
        if (ym_file_rd(file,&ch,1) == 0)
            throw ym_exception("Unexpected End of File",file->path,ym_file_get_error_code());
        if (ch == '\r')
            continue;
        if (ch == delim)
            return out;
        out += ch;
    }while (1);
}
const char* grab_until_delim(std::string &token,const char *str,char delim){
    //  Streams characters from "str" into the builder until a deliminator is found.

    char ch;
    do{
        ch = *str++;
        if (ch == '\r')
            continue;
        if (ch == delim || ch == '\0')
            return str;
        token += ch;
    }while (1);
}
ym_uL parse_uL(const char *str){
    ym_uL x = 0;
    while (*str != '\0')
        x = 10*x + (ym_pL)(*str++ - '0');
    return x;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Rule of 5
YCDFileReader::YCDFileReader(){
    file.path = NULL;
}
void YCDFileReader::operator=(YCDFileReader &&x){
    ym_file_close(&file);
    path                = std::move(x.path);
    file                = x.file;
    file_version        = std::move(x.file_version);
    radix               = x.radix;
    first_digits        = std::move(x.first_digits);
    total_digits        = x.total_digits;
    digits_per_file     = x.digits_per_file;
    file_id             = x.file_id;
    digits_per_word     = x.digits_per_word;
    words_in_this_file  = x.words_in_this_file;
    data_offset         = x.data_offset;
    x.file.path = NULL;
}
YCDFileReader::~YCDFileReader(){
    ym_file_close(&file);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Constructors
YCDFileReader::YCDFileReader(std::wstring path_)
    : path(std::move(path_))
{
    //  Open the file
    if (ym_file_open_r(NULL,&file,path.c_str()))
        throw ym_exception("Unable to open file.",path,ym_file_get_error_code());

    //  Parse the file

    //  Start after the next newline.
    {
        char ch;
        do{
            if (ym_file_rd(&file,&ch,1) == 0){
                ym_file_print_error();
                ym_file_close(&file);
                throw ym_exception("Invalid File Format",std::move(path),ym_file_get_error_code());
            }
        }while (ch != '\n');
    }

    //  Parse header info
    while (1){
        std::string token;
        try{
            token = grab_until_delim(&file,'\n');
        }catch(...){
            ym_file_close(&file);
            throw;
        }

        //  Empty line
        if (token.size() == 0)
            continue;

        //  End of header section
        if (token == YCF_CDF_TOKEN_EndHeader){
            break;
        }

        //  Break up the token.
        std::string key,value;

        //  Parse the key
        const char *ptr = token.c_str();
        ptr = grab_until_delim(key,ptr,'\t');

        //  Parse the value
        grab_until_delim(value,ptr,'\n');

        //  file_version
        if (key == YCF_CDF_TOKEN_FileVersion){
            file_version = std::move(value);
            continue;
        }

        //  radix
        if (key == YCF_CDF_TOKEN_Base){
            radix = (int)parse_uL(value.c_str());
            continue;
        }

        //  first_digits
        if (key == YCF_CDF_TOKEN_FirstDigits){
            first_digits = std::move(value);
            continue;
        }

        //  total_digits
        if (key == YCF_CDF_TOKEN_TotalDigits){
            total_digits = parse_uL(value.c_str());
            continue;
        }

        //  digits_per_file
        if (key == YCF_CDF_TOKEN_BlockSize){
            digits_per_file = (ym_fL)parse_uL(value.c_str());
            continue;
        }

        //  file_id
        if (key == YCF_CDF_TOKEN_BlockID){
            file_id = parse_uL(value.c_str());
            continue;
        }
    }

    //  Find offset
    //  This isn't efficient. It sets the file pointer back to zero and counts
    //  to the first null character.
    ym_file_setptr(&file,0);
    char ch;
    ym_uL c = 0;
    while (1){
        if (ym_file_rd(&file,&ch,1) == 0){
            throw ym_exception("Error Reading File",ym_file_get_error_code());
        }
        c++;
        if (ch == '\0')
            break;
    };
    data_offset = c;

    //  Check Version
    if (file_version.size() == 0){
        throw ym_exception("No version # found.",std::move(path),YCR_DIO_ERROR_INVALID_FILE);
    }
    if (file_version != "1.0.0" && file_version != "1.1.0"){
        throw ym_exception(
            "This .ycd file is of a later format version.\n"
            "This version of the digit viewer is unable to view this file.",
            std::move(path),
            YCR_DIO_ERROR_INVALID_FILE
        );
    }

    //  Other checks
    if (digits_per_file < 100){
        throw ym_exception("Invalid Digits per File",YCR_DIO_ERROR_INVALID_FILE);
    }

    if (total_digits != 0 && digits_per_file > total_digits)
        digits_per_file = total_digits;

    //  Boundaries
    ym_uL block_start = file_id * digits_per_file;
    ym_uL block_end   = block_start + digits_per_file;
    if (total_digits != 0){
        if (total_digits <= block_start){
            std::string error = "This file is out of range.\n";
            error += "Start:        ";
            error += std::to_string(block_start);
            error += "\n";
            error += "End:          ";
            error += std::to_string(block_end);
            error += "\n";
            error += "Total Digits: ";
            error += std::to_string(total_digits);
            error += "\n";
            throw ym_exception(error,YCR_DIO_ERROR_OUT_OF_RANGE);
        }
        if (block_end > total_digits){
            block_end = total_digits;
        }
    }
    ym_fL block_digits = (ym_fL)(block_end - block_start);

    //  Radix-specific things
    switch (radix){
        case 10:
            digits_per_word = 19;
            words_in_this_file = (block_digits - 1) / 19 + 1;
            break;
        case 16:
            digits_per_word = 16;
            words_in_this_file = (block_digits - 1) / 16 + 1;
            break;
        default:
            throw ym_exception("Unsupported Radix",YCR_DIO_ERROR_INVALID_BASE);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDFileReader::print() const{
    ymo_print_wstr(path.c_str());
    ymo_print_astr("\n\n");

    ymo_print_astr("file_version:       ");
    ymo_print_astr(file_version.c_str());
    ymo_print_astr("\n");

    ymo_print_astr("radix:              ");
    ymo_print_s64(radix);
    ymo_print_astr("\n");

    ymo_print_astr("first_digits:       ");
    ymo_print_astr(first_digits.c_str());
    ymo_print_astr("\n");

    ymo_print_astr("total_digits:       ");
    ymo_print_uL(total_digits);
    ymo_print_astr("\n");

    ymo_print_astr("digits_per_file:    ");
    ymo_print_uL(digits_per_file);
    ymo_print_astr("\n");

    ymo_print_astr("file_id:            ");
    ymo_print_uL(file_id);
    ymo_print_astr("\n\n");

    ymo_print_astr("digits_per_word:    ");
    ymo_print_uL(digits_per_word);
    ymo_print_astr("\n");

    ymo_print_astr("words_in_this_file: ");
    ymo_print_uL(words_in_this_file);
    ymo_print_astr("\n");

    ymo_print_astr("data_offset:        ");
    ymo_print_uL(data_offset);
    ymo_print_astr("\n\n");
}
void YCDFileReader::read_words(ym_fL pos,ym_u64 *T,ym_pL L){
    //  This method reads L words starting at offset "pos".
    //  "pos" is measured in 64-bit words. It is relative to the start of the
    //  data section of the .ycd file.

    //  Thus the byte region of the file that is read is:
    //      [
    //          (data section offset) + pos * sizeof(ym_u64),
    //          (data section offset) + (pos + L) * sizeof(ym_u64)
    //      )

    //  Note the [start, end). Thus it reads from "start" and ends 1 byte before
    //  "end".

    //  If any portion is out of range of the file, it throw an exception.

    if (pos + L > words_in_this_file){
        ymo_warning("Internal Error: Read out of Bounds");
        std::string error = "YCDFile::read_words(ym_fL pos,ym_u64 *T,ym_pL L)\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(pos);
        error += " - ";
        error += std::to_string(pos + L);
        error += "\nAvailable Range: ";
        error += std::to_string(0);
        error += " - ";
        error += std::to_string(words_in_this_file);
        throw ym_exception(error,YCR_DIO_ERROR_OUT_OF_RANGE);
    }

    //  Set file pointer
    ym_file_setptr(&file,data_offset + pos * sizeof(ym_u64));

    //  Read
    ym_pL words_read = ym_file_rd(&file,T,L * sizeof(ym_u64)) / sizeof(ym_u64);
    if (words_read != L){
        throw ym_exception("Error Reading File",ym_file_get_error_code());
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDFileReader::read_chars(
    ym_uL pos,char *str,ym_pL digits,
    ym_u64 *buffer,ym_pL buffer_L,
    void (*fp_convert)(char*,const ym_u64*,ym_pL)
){
    //  This method reads digits from the file.
    //  It reads the following region:
    //      [pos, pos + digits)

    //  "pos" is relative to the start of the constant. Thus it is required that
    //  the region fits entirely inside this file. Otherwise it will throw an
    //  exception.

    //Parameters:
    //  -   pos                 -   Starting digit position of the constant.
    //  -   (str, digits)       -   Output buffer
    //  -   (buffer, buffer_L)  -   Scratch memory
    //  -   fp_convert          -   Function pointer for binary -> char conversion.

    if (digits == 0)
        return;

    //  Get boundaries
    ym_uL block_start = digits_per_file * file_id;
    ym_uL block_end   = block_start + digits_per_file;
    if (total_digits != 0 && block_end > digits_per_file)
        block_end = total_digits;

    //  Check boundaries
    if (pos + digits > block_end || pos < block_start){
        std::string error = "void YCDFile::read_chars()\n";
        error += "Read out of bounds.\n";
        error += "Requested Range: ";
        error += std::to_string(pos);
        error += " - ";
        error += std::to_string(pos + digits);
        error += "\nAvailable Range: ";
        error += std::to_string(block_start);
        error += " - ";
        error += std::to_string(block_end);
        throw ym_exception(error,YCR_DIO_ERROR_OUT_OF_RANGE);
    }

    //  Local digit range.
    ym_fL local_start = (ym_fL)(pos - block_start);
    ym_fL local_end   = local_start + digits;

    //  Get word boundaries
    ym_fL word_start  = local_start / digits_per_word;
    ym_fL word_end    = (local_end + digits_per_word - 1) / digits_per_word;
    ym_pL word_length = (ym_pL)(word_end - word_start);

    //  Edge offsets
    ym_pL bot_offset = (ym_pL)(local_start - word_start  * digits_per_word);
    ym_pL top_offset = (ym_pL)(local_end - (word_end - 1) * digits_per_word);

    //  Need special case handling for only 1 word.
    //  This is because each of the edge handlers after this will only handle
    //  its own edge and write the rest of the word. So if a request fits
    //  entirely into a single word, each edge handler will do the same one
    //  and lead to writing out-of-bounds on the destination array.
    if (word_length == 1){
//        cout << "Single Word" << endl;
        char tmp[19];
        read_words(word_start,buffer,1);
        fp_convert(tmp,buffer,1);
        memcpy(str,tmp + bot_offset,top_offset - bot_offset);
        return;
    }

    //  Work loop
    while (word_length > 0){
        //  Select a block size
        ym_pL current_block = buffer_L;
        if (current_block > word_length)
            current_block = word_length;
//        cout << "current_block = " << current_block << endl;

        ym_u64 *current_buffer = buffer;

        //  Load words
        read_words(word_start,current_buffer,current_block);

        //  Bottom edge
        if (bot_offset != 0){
//            cout << "Bottom Edge: " << bot_offset << endl;
            char tmp[19];
            fp_convert(tmp,current_buffer,1);
            memcpy(str,tmp + bot_offset,digits_per_word - bot_offset);
            word_start++;
            word_length--;
            current_block--;
            current_buffer++;
            str += digits_per_word - bot_offset;

            //  This if-statement is only meant to be entered once on the very
            //  first iteration. This will prevent it from entering again.
            bot_offset = 0;
        }

        //  Top edge
        //  If this is the last iteration and the top edge is split, then
        //  we stop one word short.
        bool handle_top_edge =
            current_block == word_length &&
            top_offset != digits_per_word;
        if (handle_top_edge){
            current_block--;
        }

        //  Convert the middle words.
        if (current_block != 0){
//            cout << "Middle: " << current_block << endl;
            fp_convert(str,current_buffer,current_block);
            word_start += current_block;
            word_length -= current_block;
            current_buffer += current_block;
            str += current_block * digits_per_word;
        }

        //  Top edge
        if (handle_top_edge){
//            cout << "Top Edge: " << top_offset << endl;
            char tmp[19];
            fp_convert(tmp,current_buffer,1);
            memcpy(str,tmp,top_offset);
            word_start++;
            word_length--;
            str += top_offset;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
