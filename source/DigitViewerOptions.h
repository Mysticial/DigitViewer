/* DigitViewerOptions.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/02/2012
 * Last Modified    : 08/04/2013
 * 
 *  This file implements the various features in the Digit Viewer.
 */

#include <string.h>
#include "y-cruncher/y-cruncher.h"

using namespace DigitViewer;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Helpers
#define YM_SP_ROW_PRINT_WIDTH 79
void clearline(){
    //  Clears the a 79-character line.
    ymo_print_astr("\r");

    int c = 0;
    while (c++ < YM_SP_ROW_PRINT_WIDTH)
        ymo_print_astr(" ");

    ymo_print_astr("\r");
}
void display_char(char digit,ym_pL *b,ym_pL *r,ym_uL *c){
    //  Print a single digit.

    //Parameters:
    //  -   b       =   position in the current 10-digit block
    //  -   r       =   position in the current 50-digit row
    //  -   c       =   position from the decimal place

    (*b)++;
    (*r)++;
    (*c)++;

    ymo_set_color_Y(1);
    char str[] = {digit,'\0'};
    ymo_print_astr(str);
    ymo_set_color_W();

    if (*b == 10){
        *b = 0;
        ymo_print_wstr(L" ");
    }
    if (*r == 50){
        *r = 0;
        ymo_print_wstr(L" :  ");
        ymo_set_color_G(1);
        ymo_print_uL_commas(*c,0);
        ymo_set_color_W();
        ymo_print_astr("\n");
    }
}
void display_fancy(ym_uL pos,const char *str,ym_pL digits){
    //  Print a string of digits in fancy format.

    ym_uL end = pos + digits;

    ym_uL c = pos - pos % 50;
    ym_pL b = 0;
    ym_pL r = 0;

    //  Leading spaces
    while (c < pos)
        display_char(' ',&b,&r,&c);

    //  Print digits
    while (c < end){
        char ch = *str++;
        if (ch == '\0')
            break;
        display_char(ch,&b,&r,&c);
    }

    if (c < 50)
        ymo_print_astr("\n\n");
    else
        ymo_print_astr("\n");
}
inline ym_u64 hash_mul10(ym_u64 hash){
    hash += hash;
    hash = (hash & 0x1fffffffffffffffull) + (hash >> 61);

    hash *= 5;
    hash = (hash & 0x1fffffffffffffffull) + (hash >> 61);

    return hash;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Features
void ViewRange(DigitReader *file){
    //  Print out digits in a specific range.
    ymo_print_astr("\n\nView a Range of Digits\n\n");

    ym_uL limit = file->get_digits();
    if (limit == 0)
        limit = (ym_uL)0 - 1;

    ymo_print_astr("\n");

    ym_uL start = ymo_prompt_uL(L"Starting Digit: ",1,limit);
    ymo_print_astr("\n");
    start--;
    limit -= start;

    //  Don't print out more than 100k digits.
    if (limit > 100000)
        limit = 100000;

    ym_pL digits = (ym_pL)ymo_prompt_uL(L"Digits to View: ",1,limit);
    ymo_print_astr("\n");

    //  Check range
    if (!file->check_range(start,start + digits)){
        ymo_print_astr("This range of digits is not accessible.");
        return;
    }

    //  Set output format.
    file->set_raw(false);

    //  Read digits
    std::string str(digits,'-');
    file->read(start,&str[0],digits);

    //  Check bounds
    limit = file->get_digits();
    if (limit){
        if (start >= limit){
            ymo_print_astr("No digits to view.");
            return;
        }
        if (start + digits > limit){
            digits = (ym_pL)(limit - start);
        }
    }

    //  Display
    display_fancy(start,&str[0],digits);
}
void CountDigits(DigitReader *file){
    //  Count up the # of instances of each digit.

    ym_uL limit = file->get_digits();
    if (limit == 0)
        limit = (ym_uL)0 - 1;

    ymo_print_astr("\n");

    ym_uL start = ymo_prompt_uL(L"Starting Digit: ",1,limit);
    ym_uL end   = ymo_prompt_uL(L"Ending Digit:   ",start,limit);
    start--;

    //  Check range
    if (!file->check_range(start,end)){
        ymo_print_astr("This range of digits is not accessible.");
        return;
    }

    file->set_pos(start);
    file->set_raw(true);

    ym_u64 digit[16];
    memset(digit,0,sizeof(digit));

    ymo_print_astr("\n");

    ym_uL pos = start;
    while (pos < end){
        ym_pL block = 1000000000;
        ym_uL left = end - pos;
        if (block > left)
            block = (ym_pL)left;

        for (ym_pL c = 0; c < block; c++)
            digit[(int)file->next()]++;

        pos += block;

        //  Print Results
        ymo_print_uL_commas(start + 1,0);
        ymo_print_astr(" - ");
        ymo_print_uL_commas(pos,0);
        ymo_print_astr(" :\t");
        ymo_print_u64s(digit,file->get_radix());
    }
}
void ComputeHash(DigitReader *file){
    //  Count up the # of instances of each digit.

    ym_uL limit = file->get_digits();
    if (limit == 0)
        limit = (ym_uL)0 - 1;

    ymo_print_astr("\n");

    ym_uL end   = ymo_prompt_uL(L"Ending Digit:   ",1,limit);

    //  Check range
    if (!file->check_range(0,end)){
        ymo_print_astr("This range of digits is not accessible.");
        return;
    }

    file->set_pos(0);
    file->set_raw(true);

    ymo_print_astr("\n");

    //  Digits before decimal place
    ym_u64 hash = 0;
    {
        std::string first_digits = file->get_first_digits(64);

        bool decimal_found = false;
        for (char ch : first_digits){
            if (ch == '.'){
                decimal_found = true;
                break;
            }

            //  Accumulate hash
            hash = hash_mul10(hash);
            hash += ch - '0';
        }
        if (!decimal_found){
            throw ym_exception(
                "A decimal place was not found within the first 63 bytes of the file.",
                YCR_DIO_ERROR_NO_DECIMAL_PLACE
            );
        }
    }

    ym_uL pos = 0;
    while (pos < end){
        ym_pL block = 1000000000;
        ym_uL left = end - pos;
        if (block > left)
            block = (ym_pL)left;

        //  Accumulate the hash 1 digit at a time.
        //  y-cruncher can do this much more efficiently by directly
        //  accumulating entire 64-bit compressed words at a time - thereby
        //  completely bypassing the need to decompress the digits at all.

        //  But that code is not trivial. So we'll stick with a simple
        //  implementation here.

        //  In any case, this feature is used to verify that the digits were
        //  written properly and that the Digit Viewer correctly decompresses
        //  them. So we actually *want* to decompress the digits before
        //  computing the hash.

        for (ym_pL c = 0; c < block; c++){
            hash = hash_mul10(hash);
            hash += (int)file->next();
        }

        //  Make final correction before printing it out.
        if (hash > 0x1fffffffffffffffull)
            hash -= 0x1fffffffffffffffull;

        pos += block;

        //  Print Results
        ymo_print_uL_commas(pos,0);
        ymo_print_astr(" :\t");
        ymo_print_u64(hash);
        ymo_print_astr("\n");
    }
}
void ToTextFile(DigitReader *file){
    ymo_print_astr("\n\nWrite to Text File.\n\n");

    ym_uL limit = file->get_digits();
    if (limit == 0)
        limit = (ym_uL)0 - 1;

    ymo_print_astr("\n");

    ym_uL start = ymo_prompt_uL(L"Starting Digit: ",1,limit);
    ymo_print_astr("\n");
    start--;
    limit -= start;

    ym_uL digits = ymo_prompt_uL(L"Digits to Write: ",1,limit);
    ymo_print_astr("\n");

    //  Check range
    if (!file->check_range(start,start + digits)){
        ymo_print_astr("This range of digits is not accessible.");
        return;
    }

    //  Use plain-text output. This is faster for the text writer.
    file->set_raw(false);

    ymo_print_astr("Enter the path of the destination file:\n");

    //  Prompt for path
    wchar_t path_c[1024];
    ymo_scan_str(path_c,1024);
    std::wstring path = path_c;

    //  Extract the extension
    size_t extension_offset = path.rfind('.');

    //  Append extension if needed.
    if (extension_offset >= path.size() ||
        path.substr(extension_offset) != L".txt"
    ){
        path += L".txt";
    }

    //  Create Writer
    TextWriter writer(
        path,
        start == 0 ? file->get_first_digits(100) : "",
        false,
        file->get_radix()
    );

    //  Allocate Buffer
    ym_pL buffer_L = YC_DIGITWRITER_DEFAULT_BUFFER;
    auto str = std::unique_ptr<char[]>(new char[buffer_L]);

    ymo_print_astr("\n");

    //  Streaming Loop
    //  The fastest way to do this is to use our own buffer and call the
    //  no-buffer methods for both the reader and the writer.
    ym_uL written = 0;
    while (digits > 0){
        ym_pL current_digits = buffer_L;
        if (current_digits > digits)
            current_digits = (ym_pL)digits;

        //  Read digits
        file->read(start,str.get(),current_digits);

        //  Write digits
        writer.write(str.get(),current_digits);

        //  Update Counters
        start   += current_digits;
        digits  -= current_digits;
        written += current_digits;

        //  Print
        clearline();
        ymo_print_astr("Streaming:  ");
        ymo_print_uL_commas(written,0);
        ymo_print_astr("  digits written");
    }
}
void ToYCDFileAll(DigitReader *file){
    ymo_print_astr("\n\nWrite to Compressed File.\n\n");

    ym_uL limit = file->get_digits();
    if (limit == 0)
        limit = (ym_uL)0 - 1;

    ymo_print_astr("\n");

    ym_uL digits = ymo_prompt_uL(L"Digits to Write: ",1000000,limit);
    ymo_print_astr("\n");

    //  Check range
    if (!file->check_range(0,digits)){
        ymo_print_astr("This range of digits is not accessible.");
        return;
    }

    //  Use plain-text output. This is faster for the text writer.
    file->set_raw(true);
    
    ym_uL digits_per_file;
    do{
        digits_per_file = ymo_prompt_uL(L"Digits per file (0 for single file): ",0,digits);
        ymo_print_astr("\n");
        if (digits_per_file == 0)
            digits_per_file = (ym_uL)0 - 1;
        if (digits_per_file < 1000000){
            ymo_warning("Must be at least 1,000,000.\n\n");
            continue;
        }
        break;
    }while (1);

    wchar_t path_c[1024];
    ymo_print_astr("Enter a name for the digits:\n");
    ymo_print_astr("Name: ");
    ymo_scan_str(path_c,1024);
    std::wstring name = path_c;

    ymo_print_astr("\nEnter the destination path (SPACE for default):\n");
    ymo_scan_str(path_c,1024);
    std::wstring path = path_c;
    if (path.size() != 0 && path[0] == L' ')
        path.clear();

    //  Create the writer
    YCDWriter writer(
        std::move(path),
        std::move(name),
        file->get_first_digits(64),
        digits_per_file,
        0,
        file->get_radix()
    );

    //  Allocate Buffer
    ym_pL buffer_L = YC_DIGITWRITER_DEFAULT_BUFFER;
    auto str = std::unique_ptr<char[]>(new char[buffer_L]);

    ymo_print_astr("\n");

    //  Streaming Loop
    //  The fastest way to do this is to use our own buffer and call the
    //  no-buffer methods for both the reader and the writer.
    ym_uL current_position = 0;
    ym_uL written = 0;
    while (digits > 0){
        ym_pL current_digits = buffer_L;
        if (current_digits > digits)
            current_digits = (ym_pL)digits;

        //  Read digits
        file->read(current_position,str.get(),current_digits);

        //  Write digits
        writer.write(str.get(),current_digits);

        //  Update Counters
        current_position += current_digits;
        digits           -= current_digits;
        written          += current_digits;

        //  Print
        clearline();
        ymo_print_astr("Streaming:  ");
        ymo_print_uL_commas(written,0);
        ymo_print_astr("  digits written");
    }
}
void ToYCDFilePartial(YCDReader *file){
    ymo_print_astr("\n\nRecompress to Partial Compressed File.\n\n");

    ym_uL limit = file->get_digits();
    if (limit == 0)
        limit = (ym_uL)0 - 1;

    ymo_print_astr("\n");

    //  Use raw output.
    file->set_raw(true);
    
    ym_uL digits_per_file = ymo_prompt_uL(L"Digits per file: ",0,limit);

    ymo_print_astr("\n");
    ym_uL start_id = ymo_prompt_uL(L"Start ID: ",0,(ym_uL)0 - 1);
    ym_uL end_id   = ymo_prompt_uL(L"End   ID: ",start_id,(ym_uL)0 - 1) + 1;

    ym_uL start_pos = digits_per_file * start_id;
    ym_uL end_pos   = digits_per_file * end_id;
    if (start_pos >= limit){
        ymo_warning("\nNo Digits to Recompress.\n");
        return;
    }
    if (end_pos > limit)
        end_pos = limit;

    //  Check range
    if (!file->check_range(start_pos,end_pos)){
        ymo_print_astr("This range of digits is not accessible.");
        return;
    }

    wchar_t path_c[1024];
    ymo_print_astr("\nEnter the destination path (SPACE for default):\n");
    ymo_scan_str(path_c,1024);
    std::wstring path = path_c;
    if (path.size() != 0 && path[0] == L' ')
        path.clear();

    //  Get the name
    std::wstring name = file->get_name();

    //  Strip off the trailing " - ".
    while (name.size() > 0 && (name.back() == ' ' || name.back() == '-')){
        name.resize(name.size() - 1);
//        name.pop_back();
    }

    //  Create the writer
    YCDWriter writer(
        std::move(path),
        std::move(name),
        file->get_first_digits(64),
        digits_per_file,
        start_id,
        file->get_radix()
    );

    //  Allocate Buffer
    ym_pL buffer_L = YC_DIGITWRITER_DEFAULT_BUFFER;
    auto str = std::unique_ptr<char[]>(new char[buffer_L]);

    ymo_print_astr("\n");

    //  Streaming Loop
    //  The fastest way to do this is to use our own buffer and call the
    //  no-buffer methods for both the reader and the writer.
    ym_uL digits = end_pos - start_pos;
    ym_uL current_position = start_pos;
    ym_uL written = 0;
    while (digits > 0){
        ym_pL current_digits = buffer_L;
        if (current_digits > digits)
            current_digits = (ym_pL)digits;

        //  Read digits
        file->read(current_position,str.get(),current_digits);

        //  Write digits
        writer.write(str.get(),current_digits);

        //  Update Counters
        current_position += current_digits;
        digits           -= current_digits;
        written          += current_digits;

        //  Print
        clearline();
        ymo_print_astr("Streaming:  ");
        ymo_print_uL_commas(written,0);
        ymo_print_astr("  digits written");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
