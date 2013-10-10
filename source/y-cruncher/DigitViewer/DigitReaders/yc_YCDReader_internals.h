/* yc_YCDReader_internals.h - .ycd Reader Object
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/13/2013
 * Last Modified    : 07/31/2013
 * 
 */

#pragma once
#ifndef _yc_YCDReader_internals_H
#define _yc_YCDReader_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "yc_YCDReader_headers.h"
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
YCDReader::YCDReader(std::wstring path_,bool raw,ym_pL buffer_size)
    : max_id_length(0)
    , fp_convert(NULL)
{
    if (buffer_size < 4096)
        throw ym_exception("Requested buffer size is too small.",YCR_DIO_ERROR_INVALID_PARAMETERS);

    //  Check the file name
    if (path_.size() < 4)
        throw ym_exception("File name is too short.",path_,YCR_DIO_ERROR_INVALID_FILE);

    //  Check extension
    if (path_.substr(path_.size() - 4,4) != L".ycd")
        throw ym_exception("Invalid Extension",YCR_DIO_ERROR_INVALID_EXTENSION);

    //  Separate name and path.
    ym_pL slash_index = path_.size();
    while (slash_index > 0){
        wchar_t ch = path_[slash_index - 1];
        if (ch == '/' || ch == '\\')
            break;
        slash_index--;
    }
    std::wstring base = path_.substr(0,slash_index);
    name = path_.substr(slash_index,path_.size());

    //  Add base path to path list.
    paths.push_back(std::move(base));

    //  Separate name and ID #.
    ym_pL id_index = name.size() - 4;
    while (id_index > 0){
        wchar_t ch = name[id_index - 1];
        if (ch < '0' || '9' < ch)
            break;
        id_index--;
        max_id_length++;
    }
    name.resize(id_index);

    //  Open and parse the file header.
    current_file    = YCDFileReader(std::move(path_));
    radix           = current_file.radix;
    digits_per_word = current_file.digits_per_word;
    first_digits    = current_file.first_digits;
    total_digits    = current_file.total_digits;
    digits_per_file = current_file.digits_per_file;
    blocks_per_file = (digits_per_file + digits_per_word - 1) / digits_per_word;

    //  Set the conversion function pointers.
    set_raw(raw);

    bin_buffer_L = buffer_size / sizeof(ym_u64);
    bin_buffer = (ym_u64*)ym_ah_malloc(NULL,bin_buffer_L * sizeof(ym_u64),2*sizeof(ym_u64));
}
YCDReader::~YCDReader(){
    ym_ah_free(bin_buffer);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDReader::print() const{
    ymo_print_astr("Name:   ");
    ymo_print_wstr(name.c_str());
    ymo_print_astr("\n");
    
    ymo_print_astr("max_id_length:      ");
    ymo_print_uL(max_id_length);
    ymo_print_astr("\n\n");

    //  Search Paths
    ymo_print_astr("Search Paths:\n");
    for (const std::wstring &str : paths){
        ymo_print_astr("    ");
        ymo_print_wstr(str.c_str());
        ymo_print_astr("\n");
    }
    ymo_print_astr("\n");
    
    ymo_print_astr("Radix:              ");
    ymo_print_s64(radix);
    ymo_print_astr("\n");
    
    ymo_print_astr("digits_per_word:    ");
    ymo_print_uL(digits_per_word);
    ymo_print_astr("\n");
    
    ymo_print_astr("first_digits:       ");
    ymo_print_astr(first_digits.c_str());
    ymo_print_astr("\n\n");
    
    ymo_print_astr("total_digits:       ");
    ymo_print_uL(total_digits);
    ymo_print_astr("\n\n");
    
    ymo_print_astr("digits_per_file:    ");
    ymo_print_uL(digits_per_file);
    ymo_print_astr("\n");
    
    ymo_print_astr("blocks_per_file:    ");
    ymo_print_uL(blocks_per_file);
    ymo_print_astr("\n\n");
    
    ymo_print_astr("bin_buffer_L:       ");
    ymo_print_uL(bin_buffer_L);
    ymo_print_astr("\n");
    
    ymo_print_astr("buffer_L:           ");
    ymo_print_uL(buffer_L);
    ymo_print_astr("\n\n");

    ymo_print_astr("Iterator File Offset:   ");
    ymo_print_uL(iter_f_offset);
    ymo_print_astr("\n");

    ymo_print_astr("Iterator Buffer Offset: ");
    ymo_print_uL(iter_b_offset);
    ymo_print_astr("\n\n");
}
int YCDReader::get_radix() const{
    return radix;
}
ym_fL YCDReader::get_digits() const{
    return total_digits;
}
void YCDReader::set_raw(bool raw){
    //  Clear the cache
    clear_buffer();

    switch (radix){
        case 10:
            fp_convert = raw ? ymb_CVN_u64d_to_rawd_f : ymb_CVN_u64d_to_strd_f;
            break;
        case 16:
            fp_convert = raw ? ymb_CVN_u64b_to_rawh_f : ymb_CVN_u64b_to_strh_f;
            break;
        default:;
    }
}
bool YCDReader::check_range(ym_uL start,ym_uL end){
    //  Checks to see if all the necessary files to access the range [start, end)
    //  exist and can be opened.

    //  Find file boundaries
    ym_uL file_start = start / digits_per_file;
    ym_uL file_end   = (end + digits_per_file - 1) / digits_per_file;

    bool ret = true;

    for (ym_uL file = file_start; file < file_end; file++){
        try{
            set_current_file(file);
        }catch (ym_exception &e){
            if (e.code == YCR_DIO_ERROR_INCONSISTENT){
                ymo_warning("Corrupt or inconsistent file:\n");
            }

            if (total_digits != 0 && total_digits < end){
                ymo_warning("This digit stream does not have enough digits.\n");
                return false;
            }
            if (ret){
                ymo_warning("The following needed files are missing or inaccessible:\n");
            }
            std::wstring path = name + std::to_wstring(file) + L".ycd";
            ymo_print_wstr(path.c_str());
            ymo_print_astr("\n");
            ret = false;
        }
    }

    return ret;
}
std::string YCDReader::get_first_digits(ym_pL L){
    return first_digits;
}
void YCDReader::read(ym_uL pos,char *str,ym_pL digits){
    //  This method reads digits from the file.
    //  It reads the following region:
    //      [pos, pos + digits)

    //  "pos" is relative to the start of the constant. Thus it open as many
    //  files as needed to get that range. If any of those files are missing
    //  or fail to open, then it will throw an exception.

    if (digits == 0)
        return;

    ym_uL end = pos + digits;
    if (total_digits != 0 && end > total_digits)
        throw ym_exception("Out of range.",YCR_DIO_ERROR_OUT_OF_RANGE);

    //  Find file boundaries
    ym_uL file_start = pos / digits_per_file;
    ym_uL file_end   = (end + digits_per_file - 1) / digits_per_file;

    //  Only 1 file
    if (file_end - file_start == 1){
        set_current_file(file_start);
//        cout << file_start << "  " << pos << "   " << pos + digits << "  " << digits << endl;
        current_file.read_chars(pos,str,digits,bin_buffer,bin_buffer_L,fp_convert);
        return;
    }

    //  Read from each file.
    for (ym_uL file = file_start; file < file_end; file++){
        //  Get boundaries
        ym_uL local_start = file * digits_per_file;
        ym_uL local_end   = local_start + digits_per_file;

        //  Set file
        set_current_file(file);
//        cout << "file = " << file << endl;

        //  First file may be a partial file.
        if (file == file_start){
            ym_pL current = (ym_pL)(local_end - pos);
            current_file.read_chars(pos,str,current,bin_buffer,bin_buffer_L,fp_convert);
            str += current;
            pos += current;
            digits -= current;
            continue;
        }

        //  Top filter
        ym_pL current = digits;
        if (current > digits_per_file)
            current = (ym_pL)digits_per_file;

        current_file.read_chars(pos,str,current,bin_buffer,bin_buffer_L,fp_convert);

        str += current;
        pos += current;
        digits -= current;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void YCDReader::add_search_path(std::wstring path){
    //  Make sure path ends in a slash.
    if (path.size() != 0){
        wchar_t back = path.back();
        if (back != '/' && back != '\\')
            path += '/';
    }
    paths.push_back(std::move(path));
}
const std::wstring& YCDReader::get_name() const{
    return name;
}
ym_fL YCDReader::get_digits_per_file() const{
    return digits_per_file;
}
void YCDReader::print_paths() const{
    for (const std::wstring &str : paths){
        ymo_print_astr("    ");
        ymo_print_wstr(str.c_str());
        ymo_print_astr("\n");
    }
}
ym_pL YCDReader::get_num_paths() const{
    return paths.size();
}
void YCDReader::load_new_file(std::wstring path,ym_uL id){
    //  Loads a new file, checks all the metadata and updates "total_digits" and
    //  "max_id_length" if possible.

    //  Get id length.
    std::wstring id_string = std::to_wstring(id);
    ym_pL query_id_length = id_string.size();

    //  Open the file.
    YCDFileReader new_file(std::move(path));

    //  Cross check all the metadata.
    if (new_file.file_version != current_file.file_version)
        throw ym_exception("File version does not match.",YCR_DIO_ERROR_INCONSISTENT);
    if (new_file.radix != current_file.radix)
        throw ym_exception("Radix does not match.",YCR_DIO_ERROR_INCONSISTENT);
    if (new_file.digits_per_file != current_file.digits_per_file)
        throw ym_exception("Digits per file does not match.",YCR_DIO_ERROR_INCONSISTENT);
    if (new_file.digits_per_word != current_file.digits_per_word)
        throw ym_exception("Digits per word does not match.",YCR_DIO_ERROR_INCONSISTENT);
    if (total_digits != 0 &&
        current_file.total_digits != 0 &&
        total_digits != current_file.total_digits
    ){
        throw ym_exception("Total digit does not match.",YCR_DIO_ERROR_INCONSISTENT);
    }

    //  Set new total digits.
    if (total_digits == 0)
        total_digits = new_file.total_digits;

    //  Update maximum id length.
    if (max_id_length < query_id_length)
        max_id_length = query_id_length;

    current_file = std::move(new_file);
}
void YCDReader::set_current_file(ym_uL id){
    //  Open the specified file id.
    //  This funtion will search all the search paths and will try a variety of
    //  different zero-padded number schemes. If this function successfully
    //  opens a file with an id that is longer than the current "max_id_length",
    //  it will update that value to reflect that new longest id.

    //  Already on the right file.
    if (current_file.file_id == id)
        return;

    std::wstring id_string = std::to_wstring(id);
    ym_pL query_id_length = id_string.size();
    ym_pL limit_id_length = query_id_length > max_id_length ? query_id_length : max_id_length;

    //  For each search path:
    for (std::wstring &path : paths){
        //  Try all zero-padded schemes up to the maximum length.
        std::wstring base_path = path + name;
        ym_pL id_length = query_id_length;
        while (true){
            //  Build the full path.
            std::wstring full_path = base_path + id_string + L".ycd";
//            ymo_print_wstr(full_path.c_str()); cout << endl;

            //  If the file exists, we're done. Open it.
            if (ym_file_exist(full_path.c_str())){
                load_new_file(std::move(full_path),id);
                return;
            }

            //  File not found after searching all zero-padded lengths.
            if (id_length == limit_id_length)
                break;

            //  Add another leading zero.
            id_length++;
            base_path += '0';
        }
    }

    std::string error = "Unable to open file #";
    error += std::to_string(id);
    throw ym_exception(error,YCR_DIO_ERROR_FILE_NOT_FOUND);
}
YM_NOINLINE void YCDReader::reload(){
    //  This overrides the default reload() function. This is necessary because
    //  the default reload() will try to load as much as possible.
    //
    //  However, loading too much will cause an exception due to going out of
    //  bounds. And the extended file formats mean that you don't know how
    //  many digits are left.
    //
    //  This override will load as much as possible, but it will never cross
    //  a file boundary. So it will load at most to the end of a file.


    //  The function is mostly copy/paste from DigitViewer::reload() with
    //  the file-limiting code injected into the middle.

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

    ym_pL read  = buffer_L;
    ym_uL start = iter_f_offset;
    ym_uL end   = iter_f_offset + read;
    if (total_digits != 0 && start >= total_digits){
        std::string error("No more digits left: ");
        error += std::to_string(total_digits);
        throw ym_exception(std::move(error),YCR_DIO_ERROR_NO_DIGITS_LEFT);
    }

    //  Get read limit.
    ym_uL read_offset_limit = end;

    //  Don't cross a file boundary.
    ym_uL file_id = start / digits_per_file;
    ym_uL file_limit = (file_id + 1) * digits_per_file;
    if (read_offset_limit > file_limit){
        read_offset_limit = file_limit;
    }

    //  Set the current file now. This will update "total_digits" in time for
    //  the next check.
    //  This very important because the required file could be an incomplete
    //  file that knows the end of this digit stream. Without that information
    //  it is possible for this function to fetch too many digits. This will
    //  cause "read()" to throw an exception since it will not tolerate any
    //  reads that are out of bounds.
    set_current_file(file_id);

    //  Don't overrun total digits.
    if (total_digits != 0 && read_offset_limit > total_digits){
        read_offset_limit = total_digits;
    }

    //  Calculate how much to actually read.
    ym_pL current_b_offset = 0;
    if (end > read_offset_limit){
        read = (ym_pL)(read_offset_limit - start);
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
}
#endif
