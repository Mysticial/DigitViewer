/* ConsoleIO_Posix.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/26/2014
 * Last Modified    : 08/26/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include <iostream>
#include "../StringTools/ToString.h"
#include "Label.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void CompileOptions(){
    Console::println_labelm("Console IO", "Color Codes", 'G');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Core I/O
upL_t print(const char* str, char color){
    SetColor(color);
    if (fwide(stdout, 0) <= 0){
        std::cout << str;
    }else{
        std::wcout << str;
    }
    fflush(stdout);
    return strlen(str);
}
upL_t print(const wchar_t* str, char color){
    SetColor(color);
    if (fwide(stdout, 0) <= 0){
        std::cout << StringTools::w_to_a_direct(str);
    }else{
        std::wcout << str;
    }
    fflush(stdout);
    return wcslen(str);
}
std::string scan_astr(char color){
    if (fwide(stdin, 0) > 0)
        return StringTools::w_to_a_direct(scan_wstr(color));

    SetColor(color);
    std::string out;
    std::getline(std::cin, out);
    if (color != ' ')
        SetColor('w');
    return out;
}
std::wstring scan_wstr(char color){
    if (fwide(stdin, 0) <= 0)
        return StringTools::a_to_w_direct(scan_astr(color));

    SetColor(color);
    std::wstring out;
    wchar_t ch;
    while (true){
        ch = fgetwc(stdin);
        if (ch == '\n')
            break;
        out.push_back(ch);
    }
    if (color != ' ')
        SetColor('w');
    return out;
}
void Pause(char color){
    print("Press ENTER to continue . . .", color);
    scan_astr();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Colors
void SetColor(char color){
    if (!EnableColors || color == ' ')
        return;
    const char* color_string;
    switch (color){
        case 'R':
            color_string = "\033[01;31m";
            break;
        case 'r':
            color_string = "\033[22;31m";
            break;
        case 'Y':
            color_string = "\033[01;33m";
            break;
        case 'y':
            color_string = "\033[22;33m";
            break;
        case 'G':
            color_string = "\033[01;32m";
            break;
        case 'g':
            color_string = "\033[22;32m";
            break;
        case 'B':
            color_string = "\033[01;34m";
            break;
        case 'b':
            color_string = "\033[22;34m";
            break;
        case 'T':
            color_string = "\033[01;36m";
            break;
        case 't':
            color_string = "\033[22;36m";
            break;
        case 'P':
            color_string = "\033[01;35m";
            break;
        case 'p':
            color_string = "\033[22;35m";
            break;
        default:
            color_string = "\033[01;37m";
    }
    print(color_string);
}
void SetColorDefault(){
    print("\033[39;49m");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
