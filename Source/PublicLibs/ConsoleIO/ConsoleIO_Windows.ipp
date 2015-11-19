/* ConsoleIO_Windows.ipp
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
#include <iostream>
#include <Windows.h>
#include "PublicLibs/CompilerSettings.h"
#include "Label.h"
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE void CompileOptions(){
    Console::println_labelm("Console IO", "WinAPI", 'G');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Core I/O
YM_NO_INLINE upL_t print(const char* str, char color){
    SetColor(color);
    std::cout << str;
    fflush(stdout);
    return strlen(str);
}
YM_NO_INLINE upL_t print(const wchar_t* str, char color){
    SetColor(color);
    std::wcout << str;
    fflush(stdout);
    return wcslen(str);
}
YM_NO_INLINE std::string scan_astr(char color){
    SetColor(color);
    std::string out;
    std::getline(std::cin, out);
    if (color != ' ')
        SetColor('w');
    return out;
}
YM_NO_INLINE std::wstring scan_wstr(char color){
    SetColor(color);
    std::wstring out;
    wchar_t ch;
    while (true){
        wscanf_s(L"%c", &ch, 1);
        if (ch == '\n')
            break;
        out.push_back(ch);
    }
    if (color != ' ')
        SetColor('w');
    return out;
}
YM_NO_INLINE void Pause(char color){
    SetColor(color);
    system("pause");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Console Colors
YM_NO_INLINE void SetColor(char color){
    if (!EnableColors || color == ' ')
        return;
    WORD attributes;
    switch (color){
        case 'R':
            attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case 'r':
            attributes = FOREGROUND_RED;
            break;
        case 'Y':
            attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case 'y':
            attributes = FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case 'G':
            attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case 'g':
            attributes = FOREGROUND_GREEN;
            break;
        case 'B':
            attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case 'b':
            attributes = FOREGROUND_BLUE;
            break;
        case 'T':
            attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case 't':
            attributes = FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case 'P':
            attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case 'p':
            attributes = FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        default:
            attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attributes);
}
YM_NO_INLINE void SetColorDefault(){
    SetColor('w');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
