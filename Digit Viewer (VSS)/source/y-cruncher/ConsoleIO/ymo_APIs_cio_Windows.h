/* ymo_APIs_cio_Windows.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/20/2011
 * Last Modified    : 04/20/2011
 * 
 * 
 *      ymo     =   I/O Function
 *      APIs    =   APIs
 *      cio     =   Console I/O
 *      Windows =   Windows
 * 
 */

#pragma once
#ifndef _ymo_APIs_cio_Windows_H
#define _ymo_APIs_cio_Windows_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "ymo_APIs_cio_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void yma_APIs_cio(){
    ymo_print_label("Console IO:    \t","WinAPI",'G');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Basic Printing
void ymo_print_astr(const char *str){
    printf("%s",str);
    fflush(stdout);
}
YM_NO_INLINE void ymo_print_wstr(const wchar_t *str){
    //  Global print function with Unicode support.

    UINT console_output = GetConsoleOutputCP();

    char buffer[2048];
    DWORD buffer_size = WideCharToMultiByte(console_output,0,str,-1,NULL,0,NULL,NULL);
    if (buffer_size > 2046){
        wprintf(L"Error Printing To Console");
#ifdef YCC_ENABLE_CRASH_DEBUGGING
        ym_pause();
        int *p = NULL;
        *p = 0;
#endif
        ym_quit(1);
    }

    WideCharToMultiByte(console_output, 0, str, -1, buffer, buffer_size, NULL, NULL);

    HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteFile(outHandle, buffer, buffer_size - 1, &buffer_size, 0);

    fflush(stdout);
}
void ymo_scan_str(wchar_t *buffer,ym_pL L){
    if (L == 0)
        return;
    ymo_set_color_T(1);
    ym_pL c = 0;
    while (c < L - 1){
        wscanf_s(L"%c", buffer, 1);
        if (buffer[0] < 32){
            if (c == 0)
                continue;
            break;
        }
        
        buffer++;
        c++;
    }
    ymo_set_color_W();
    *buffer = L'\0';
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Colors
void ymo_set_color_default(){
    ymo_set_color_W();
}
void ymo_set_color_W(){
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console,
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
    );
}
void ymo_set_color_R(int intense){
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console,
        FOREGROUND_RED |
        (intense ? FOREGROUND_INTENSITY : 0)
    );
}
void ymo_set_color_Y(int intense){
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console,
        FOREGROUND_RED |
        FOREGROUND_GREEN |
        (intense ? FOREGROUND_INTENSITY : 0)
    );
}
void ymo_set_color_G(int intense){
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console,
        FOREGROUND_GREEN |
        (intense ? FOREGROUND_INTENSITY : 0)
    );
}
void ymo_set_color_B(int intense){
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console,
        FOREGROUND_BLUE |
        (intense ? FOREGROUND_INTENSITY : 0)
    );
}
void ymo_set_color_T(int intense){
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console,
        FOREGROUND_GREEN | FOREGROUND_BLUE |
        (intense ? FOREGROUND_INTENSITY : 0)
    );
}
void ymo_set_color_P(int intense){
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console,
        FOREGROUND_RED | FOREGROUND_BLUE |
        (intense ? FOREGROUND_INTENSITY : 0)
    );
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Misc.
void ymo_print_ptr(void *x){
    wprintf(L"%p",x);
}
void ymo_print_sF(ym_sF x){
    wprintf(L"%g",x);
}
void ymo_print_sF_d3(ym_sF x){
    wprintf(L"%0.3f",x);
}
void ymo_print_sF_d10(ym_sF x){
    wprintf(L"%0.10f",x);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
