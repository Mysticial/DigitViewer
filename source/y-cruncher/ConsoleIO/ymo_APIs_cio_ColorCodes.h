/* ymo_APIs_cio_ColorCodes.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/20/2011
 * Last Modified    : 04/20/2011
 * 
 * 
 *      ymo         =   I/O Function
 *      APIs        =   APIs
 *      cio         =   Console I/O
 *      ColorCodes  =   ASCII Color Codes
 * 
 */

#pragma once
#ifndef _ymo_APIs_cio_ColorCodes_H
#define _ymo_APIs_cio_ColorCodes_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <wchar.h>
#include "ymo_APIs_cio_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void yma_APIs_cio(){
    ymo_print_label("Console IO:    \t","POSIX",'G');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Basic Printing
void ymo_print_astr(const char *str){
    int wide = fwide(stdout,0);

    if (wide > 0){
        while (*str != '\0'){
            wprintf(L"%c",(wchar_t)*str);
            str++;
        }
    }else{
        while (*str != '\0'){
            printf("%c",(char)*str);
            str++;
        }
    }

    fflush(stdout);
}
void ymo_print_wstr(const wchar_t *str){
    //  Global print function with Unicode support.
    int wide = fwide(stdout,0);

    if (wide > 0){
        while (*str != L'\0'){
            wprintf(L"%c",(wchar_t)*str);
            str++;
        }
    }else{
        while (*str != L'\0'){
            printf("%c",(char)*str);
            str++;
        }
    }

    fflush(stdout);
}
void ymo_scan_str(wchar_t *buffer,ym_pL L){
    if (L == 0)
        return;
    
    int wide = fwide(stdin,0);

    ymo_set_color_T(1);
    if (wide > 0){
        ym_pL c = 0;
        while (c < L - 1){
            *buffer = fgetwc(stdin);
            if (buffer[0] < 32){
                if (c == 0)
                    continue;
                break;
            }
        
            buffer++;
            c++;
        }
    }else{
        ym_pL c = 0;
        while (c < L - 1){
            *buffer = (wchar_t)fgetc(stdin);
            if (buffer[0] < 32){
                if (c == 0)
                    continue;
                break;
            }
        
            buffer++;
            c++;
        }
    }

    ymo_set_color_W();
    *buffer = L'\0';
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Colors
void ymo_set_color_W(){
    int wide = fwide(stdout,0);
    
    if (wide > 0)
        wprintf(L"\033[01;37m");
    else
        printf("\033[01;37m");
    fflush(stdout);
}
void ymo_set_color_R(int intense){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        if (intense)
            wprintf(L"\033[01;31m");
        else
            wprintf(L"\033[22;31m");
    }else{
        if (intense)
            printf("\033[01;31m");
        else
            printf("\033[22;31m");
    }
    fflush(stdout);
}
void ymo_set_color_Y(int intense){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        if (intense)
            wprintf(L"\033[01;33m");
        else
            wprintf(L"\033[22;33m");
    }else{
        if (intense)
            printf("\033[01;33m");
        else
            printf("\033[22;33m");
    }
    fflush(stdout);
}
void ymo_set_color_G(int intense){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        if (intense)
            wprintf(L"\033[01;32m");
        else
            wprintf(L"\033[22;32m");
    }else{
        if (intense)
            printf("\033[01;32m");
        else
            printf("\033[22;32m");
    }
    fflush(stdout);
}
void ymo_set_color_B(int intense){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        if (intense)
            wprintf(L"\033[01;34m");
        else
            wprintf(L"\033[22;34m");
    }else{
        if (intense)
            printf("\033[01;34m");
        else
            printf("\033[22;34m");
    }
    fflush(stdout);
}
void ymo_set_color_T(int intense){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        if (intense)
            wprintf(L"\033[01;36m");
        else
            wprintf(L"\033[22;36m");
    }else{
        if (intense)
            printf("\033[01;36m");
        else
            printf("\033[22;36m");
    }
    fflush(stdout);
}
void ymo_set_color_P(int intense){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        if (intense)
            wprintf(L"\033[01;35m");
        else
            wprintf(L"\033[22;35m");
    }else{
        if (intense)
            printf("\033[01;35m");
        else
            printf("\033[22;35m");
    }
    fflush(stdout);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Misc.
void ymo_print_ptr(void *x){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        wprintf(L"%p",x);
    }else{
        printf("%p",x);
    }
}
void ymo_print_sF(ym_sF x){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        wprintf(L"%g",x);
    }else{
        printf("%g",x);
    }
}
void ymo_print_sF_d3(ym_sF x){
    int wide = fwide(stdout,0);
    
    if (wide > 0){
        wprintf(L"%0.3f",x);
    }else{
        printf("%0.3f",x);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
