/* BasicIO.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/31/2014
 * Last Modified    : 08/31/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <cmath>
#include "../CompilerSettings.h"
#include "../StringTools/ToString.h"
#include "BasicIO.h"
////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "ConsoleIO_Windows.ipp"
#else
#include "ConsoleIO_Posix.ipp"
#endif
////////////////////////////////////////////////////////////////////////////////
namespace ymp{
namespace Console{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool EnableColors = true;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Basic Derived
upL_t print(const std::string& str, char color){
    return print(str.c_str(), color);
}
upL_t print(const std::wstring& str, char color){
    return print(str.c_str(), color);
}
upL_t println(){
    return print("\n");
}
////////////////////////////////////////////////////////////////////////////////
upL_t println(std::string str, char color){
    str += '\n';
    return print(str.c_str(), color);
}
upL_t println(std::wstring str, char color){
    str += '\n';
    return print(str.c_str(), color);
}
////////////////////////////////////////////////////////////////////////////////
void ClearLine(int characters){
    std::string x;
    x += '\r';
    x.resize(characters + 1, ' ');
    x += '\r';
    print(x);
}
void Warning(std::string str, bool sticky){
    println();
    println(std::move(str), 'R');
    if (!sticky)
        SetColor('w');
}
void Quit(int code){
    SetColorDefault();
    Pause();
    exit(code);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Integer
upL_t print(uiL_t x, char color){
    return print(StringTools::tostr(x, StringTools::NORMAL), color);
}
upL_t print(siL_t x, char color){
    return print(StringTools::tostr(x, StringTools::NORMAL), color);
}
upL_t println(uiL_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
upL_t println(siL_t x, char color){
    return println(StringTools::tostr(x, StringTools::NORMAL), color);
}
upL_t print_commas(uiL_t x, char color){
    return print(StringTools::tostr(x, StringTools::COMMAS), color);
}
upL_t print_commas(siL_t x, char color){
    return print(StringTools::tostr(x, StringTools::COMMAS), color);
}
upL_t println_commas(uiL_t x, char color){
    return println(StringTools::tostr(x, StringTools::COMMAS), color);
}
upL_t println_commas(siL_t x, char color){
    return println(StringTools::tostr(x, StringTools::COMMAS), color);
}
upL_t print_bytes(uiL_t x, char color){
    return print(StringTools::tostr(x, StringTools::BYTES), color);
}
upL_t println_bytes(uiL_t x, char color){
    return println(StringTools::tostr(x, StringTools::BYTES), color);
}
upL_t print_ebytes(uiL_t x, char color){
    return print(StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
upL_t println_ebytes(uiL_t x, char color){
    return println(StringTools::tostr(x, StringTools::BYTES_EXPANDED), color);
}
siL_t scan_siL(char color){
    auto str = scan_astr(color);
    const char* iter = str.c_str();

    siL_t out = 0;
    bool negative = false;

    if (iter[0] == '-'){
        negative = true;
        iter++;
    }

    do{
        char ch = *iter++;
        if ('0' > ch || ch > '9')
            break;
    
        out *= 10;
        out += ch - '0';
    }while (true);

    if (negative)
        out = -out;
    return out;
}
uiL_t scan_bytes(char color){
    auto str = scan_astr(color);

    uiL_t x = 0;
    upL_t c = 0;
    wchar_t ch = str[c++];
    while (ch >= L'0' && ch <= L'9'){
        x *= 10;
        x += ch - L'0';
        ch = str[c++];
    }

    double frac = 0;
    if (ch == '.'){
        double f = 0.1;

        ch = str[c++];
        while (ch >= L'0' && ch <= L'9'){
            frac += f * (ch - L'0');
            f *= 0.1;
            ch = str[c++];
        }
    }

    while (ch == ' ')
        ch = str[c++];

    switch (ch){
        case L'k':
        case L'K':
            return (x << 10) + (uiL_t)std::round(frac * ((uiL_t)1 << 10));
        case L'm':
        case L'M':
            return (x << 20) + (uiL_t)std::round(frac * ((uiL_t)1 << 20));
        case L'g':
        case L'G':
            return (x << 30) + (uiL_t)std::round(frac * ((uiL_t)1 << 30));
        case L't':
        case L'T':
            return (x << 40) + (uiL_t)std::round(frac * ((uiL_t)1 << 40));
        case L'p':
        case L'P':
            return (x << 50) + (uiL_t)std::round(frac * ((uiL_t)1 << 50));
        case L'e':
        case L'E':
            return (x << 60) + (uiL_t)std::round(frac * ((uiL_t)1 << 60));
        default:
            return x;
    };
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Floating-Point
upL_t print_float(double x, int precision, char color){
    return print(StringTools::tostr_float(x, precision), color);
}
upL_t println_float(double x, int precision, char color){
    return println(StringTools::tostr_float(x, precision), color);
}
upL_t print_fixed(double x, int precision, char color){
    return print(StringTools::tostr_fixed(x, precision), color);
}
upL_t println_fixed(double x, int precision, char color){
    return println(StringTools::tostr_fixed(x, precision), color);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Unit Tests
void print_test(bool x){
    if (x){
        Console::print("Passed", 'G');
    }else{
        Console::print("Failed", 'R');
    }
    Console::SetColor('w');
}
void println_test(bool x){
    if (x){
        Console::println("Passed", 'G');
    }else{
        Console::println("Failed", 'R');
    }
    Console::SetColor('w');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
