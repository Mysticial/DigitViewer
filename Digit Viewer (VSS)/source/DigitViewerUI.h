/* DigitViewerUI.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/02/2012
 * Last Modified    : 08/03/2013
 * 
 *  This file implements the UI of the Digit Viewer.
 */

#include "DigitViewerOptions.h"

#if (defined _M_X64) || (defined __x86_64__)
#ifdef ymc_ISA_x86_SSE41
#define YMC_BUILD_NAME      "x64 SSE4.1"
#define ymc_ISA_check   can_run_x64_SSE41
#define ymc_ISA_required    "    x64, SSE, SSE2, SSE3, SSSE3, SSE4.1"
#else
#define YMC_BUILD_NAME      "x64 SSE2"
#define ymc_ISA_check   can_run_x64_SSE2
#define ymc_ISA_required    "    x64, SSE, SSE2"
#endif
#else
#ifdef ymc_ISA_x86_SSE2
#define YMC_BUILD_NAME      "x86 SSE2"
#define ymc_ISA_check   can_run_x86_SSE2
#define ymc_ISA_required    "    SSE, SSE2"
#else
#define YMC_BUILD_NAME      "x86"
#define ymc_ISA_check   can_run_x86
#define ymc_ISA_required    "    none"
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
//  Note:
//      For the sake of consistency, the DigitViewer will use the Console I/O
//  interface from y-cruncher itself rather than the standard C++ ones.
//
//  The reason for this is that y-cruncher's Console I/O library has built-in
//  support for colors and will automatically handle stream orientation.
//
//  y-cruncher's I/O library should work correctly regardless of the stream
//  orientation.
//

using namespace DigitViewer;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Individual Options
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Type-specific Viewer Menus
void Menu_TextFile(TextReader *reader){
    ymo_print_astr("\n\nText File\n\n");
    
    ymo_print_astr("Total Digits after decimal place: ");
    ymo_set_color_G(1);
    ymo_print_uL_commas(reader->get_digits(),0);
    ymo_set_color_W();
    ymo_print_astr("\n\n");

    ymo_print_astr("First Digits:\n");
    ymo_set_color_Y(1);
    ymo_print_astr(reader->get_first_digits(50).c_str());
    ymo_set_color_W();
    ymo_print_astr("\n\n\n");

    ymo_print_astr("What would you like to do?\n\n");
    ymo_print_astr("  0     ");
    ymo_set_color_G(1);
    ymo_print_astr("View a small range of digits.\n");
    ymo_set_color_W();
    ymo_print_astr("  1     ");
    ymo_set_color_G(1);
    ymo_print_astr("Count up the # of instances of each digit.\n");
    ymo_set_color_W();

    if (reader->get_radix() == 10){
        ymo_print_astr("  2     ");
        ymo_set_color_G(1);
        ymo_print_astr("Compute hash: Floor(x * 10^digits) mod (2^61 - 1)\n");
        ymo_set_color_W();
    }

    ymo_print_astr("  3     ");
    ymo_set_color_G(1);
    ymo_print_astr("Compress the digits into one or more .ycd file.\n");
    ymo_set_color_W();

    ymo_print_astr("\nEnter your choice:\n");
    ym_pL c = (ym_pL)ymo_prompt_uL(L"option: ",0,3);
    ymo_print_astr("\n");

    switch (c){
        case 0:
            ViewRange(reader);
            break;
        case 1:
            CountDigits(reader);
            break;
        case 2:
            if (reader->get_radix() == 10)
                ComputeHash(reader);
            break;
        case 3:
            ToYCDFileAll(reader);
            break;
        default:;
    }
}
void Menu_YCDFile(YCDReader *reader){
    while (true){
        ymo_print_astr("\n\nCompressed Digit File\n\n");

        ymo_print_astr("Digits Found:\n");
        ymo_set_color_G(1);
        ym_uL digits = reader->get_digits();
        if (digits == 0){
            ymo_print_astr("Unknown - Extended File Format");
        }else{
            ymo_print_uL_commas(digits,0);
        }
        ymo_set_color_W();
        ymo_print_astr("\n\n");

        ymo_print_astr("First Digits:\n");
        ymo_set_color_Y(1);
        ymo_print_astr(reader->get_first_digits(64).c_str());
        ymo_set_color_W();
        ymo_print_astr("\n\n");

        ymo_print_astr("Digits per File:\n");
        ymo_set_color_Y(1);
        ymo_print_uL_commas(reader->get_digits_per_file(),0);
        ymo_set_color_W();
        ymo_print_astr("\n\n");

        ymo_print_astr("Search paths:\n");
        ymo_set_color_Y(1);
        reader->print_paths();
        ymo_set_color_W();
        ymo_print_astr("\n");

        ymo_print_astr("What would you like to do?\n\n");
        ymo_print_astr("  0     ");
        ymo_set_color_G(1);
        ymo_print_astr("View a small range of digits.\n");
        ymo_set_color_W();
        ymo_print_astr("  1     ");
        ymo_set_color_G(1);
        ymo_print_astr("Count up the # of instances of each digit.\n");
        ymo_set_color_W();

        if (reader->get_radix() == 10){
            ymo_print_astr("  2     ");
            ymo_set_color_G(1);
            ymo_print_astr("Compute hash: Floor(x * 10^digits) mod (2^61 - 1)\n");
            ymo_set_color_W();
        }

        ymo_print_astr("  3     ");
        ymo_set_color_G(1);
        ymo_print_astr("Decompress a range of digits to a text file.\n");
        ymo_set_color_W();
        ymo_print_astr("  4     ");
        ymo_set_color_G(1);
        ymo_print_astr("Recompress a range of digits to one or more .ycd files.\n");
        ymo_set_color_Y(1);
        ymo_print_astr("        (Advanced - Don't use unless you know what you're doing...)\n");
        ymo_set_color_W();
        ymo_print_astr("\n");
        ymo_print_astr("  5     ");
        ymo_set_color_G(1);
        ymo_print_astr("Add search directory.");
        ymo_set_color_Y(1);
        ymo_print_astr(" (if .ycd files are in multiple paths)\n");
        ymo_set_color_W();

        ymo_print_astr("\nEnter your choice:\n");
        ym_pL c = (ym_pL)ymo_prompt_uL(L"option: ",0,5);

        switch (c){
            case 0:
                ViewRange(reader);
                return;
            case 1:
                CountDigits(reader);
                return;
            case 2:
                if (reader->get_radix() == 10)
                    ComputeHash(reader);
                return;
            case 3:
                ToTextFile(reader);
                return;
            case 4:
                ToYCDFilePartial(reader);
                return;
            case 5:
            {
                ymo_print_astr("\nEnter directory:\n");
                wchar_t path[1024];
                ymo_scan_str(path,1024);
                reader->add_search_path(path);
                break;
            }
            default:;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Sub-Menus
void Menu_DigitViewer(){
    ymo_print_astr("\n\nDigit Viewer\n\n");

    ymo_print_astr("Enter the path of the file (include .txt or .ycd extensions):\n");

    //  Prompt for path
    wchar_t path_c[1024];
    ymo_scan_str(path_c,1024);
    std::wstring path = path_c;

    auto reader = OpenDigitFile(std::move(path));

    TextReader *text = dynamic_cast<TextReader*>(reader.get());
    if (text != NULL){
        Menu_TextFile(text);
        return;
    }
    YCDReader  *ycd  = dynamic_cast<YCDReader *>(reader.get());
    if (ycd != NULL){
        Menu_YCDFile(ycd);
        return;
    }
}
void CompilationOptions(){
    ymo_print_astr("\n\n");
    ymo_print_astr("================================================================\n");
    ymo_print_astr("Compilation Options\n\n");

#if defined __INTEL_COMPILER
    ymo_print_label("Compiler:           ","Intel Compiler",'T');
    ymo_print_label_uL("Version:            ",__INTEL_COMPILER,"",'T');
#elif defined _MSC_VER
    ymo_print_label("Compiler:           ","Visual C++",'T');
    ymo_print_label_uL("Version:            ",_MSC_VER,"",'T');
#elif defined __GNUC__
    ymo_print_label("Compiler:           ","GCC",'T');
    ymo_print_label_uL("Version:            ",__GNUC__,"",'T');
#else
    ymo_print_label("Compiler:           ","Unknown",'R');
    ymo_print_label("Version:            ","Unknown",'R');
#endif
    ymo_print_label("Compilation Date:   ",__DATE__,'Y');
    ymo_print_astr("\n");

    ymo_print_astr("================================================================\n");
    ymo_print_astr("Digit Conversion\n\n");

    yma_CVN_u64_forward();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Main Menu
void Menu_Main(){
    ymo_set_color_P(1);
    ymo_print_astr(PROGRAM_NAME);
    ymo_print_astr("\n\n");
    ymo_set_color_W();

    ymo_set_color_T(0);
    ymo_print_astr("Copyright ");
    ymo_print_astr("2008-2015 Alexander J. Yee");
    ymo_print_astr("      ");
    ymo_print_astr("( a-yee@u.northwestern.edu )");
    ymo_print_astr("\n\n");

    ymo_set_color_W();
    ymo_print_astr("Distribute Freely - Please Report any Bugs");
    ymo_print_astr("\n\n");
    ymo_print_astr("Version: ");
    ymo_set_color_G(1);
    ymo_print_astr(YMC_BUILD_NAME);
    ymo_print_astr("\n");
    ymo_set_color_W();

    ymo_print_astr("\n\n\n");

    ymo_print_astr("  0     ");
    ymo_set_color_G(1);
    ymo_print_astr("Digit Viewer\n");
    ymo_set_color_W();
    //ymo_print_astr("  1     ");
    //ymo_set_color_G(1);
    //ymo_print_astr("Digit Compare\n");
    //ymo_set_color_W();

    ymo_print_astr("\n");
    ymo_print_astr("  2     ");
    ymo_set_color_G(1);
    ymo_print_astr("Compilation Options\n");
    ymo_set_color_W();

    ymo_print_astr("\n\n\n\n\n\n\n");

    ymo_print_astr("\nEnter your choice:\n");
    ym_pL c = (ym_pL)ymo_prompt_uL(L"option: ",0,2);

    switch (c){
        case 0:
            Menu_DigitViewer();
            break;
        //case 1:
        //    Menu_DigitCompare();
        //    break;
        case 2:
            CompilationOptions();
            break;
        default:;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
