/* ymo_APIs_cio_Shared.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/20/2011
 * Last Modified    : 04/20/2011
 * 
 * 
 *      ymo     =   I/O Function
 *      APIs    =   APIs
 *      cio     =   Console I/O
 *      Shraed  =   Shared
 * 
 */

#pragma once
#ifndef _ymo_APIs_cio_Shared_H
#define _ymo_APIs_cio_Shared_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <math.h>
#include "ymo_APIs_cio_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymo_set_color(char color){
    switch (color){
        case 'R':
            ymo_set_color_R(1);
            break;
        case 'r':
            ymo_set_color_R(0);
            break;
        case 'Y':
            ymo_set_color_Y(1);
            break;
        case 'y':
            ymo_set_color_Y(0);
            break;
        case 'G':
            ymo_set_color_G(1);
            break;
        case 'g':
            ymo_set_color_G(0);
            break;
        case 'B':
            ymo_set_color_B(1);
            break;
        case 'b':
            ymo_set_color_B(0);
            break;
        case 'T':
            ymo_set_color_T(1);
            break;
        case 't':
            ymo_set_color_T(0);
            break;
        case 'P':
            ymo_set_color_P(1);
            break;
        case 'p':
            ymo_set_color_P(0);
            break;
        default:
            ymo_set_color_W();
    }
}
void ymo_print_label(const char *label,const char *value,char color){
    ymo_set_color_W();
    ymo_print_astr(label);
    ymo_set_color(color);
    ymo_print_astr(value);
    ymo_set_color_W();
    ymo_print_astr("\n");
}
void ymo_print_label_uL(const char *label,ym_uL value,const char *units,char color){
    ymo_set_color_W();
    ymo_print_astr(label);
    ymo_set_color(color);
    ymo_print_uL(value);
    ymo_print_astr(" ");
    ymo_print_astr(units);
    ymo_set_color_W();
    ymo_print_astr("\n");
}
void ymo_print_label_uL_commas(const char *label,ym_uL value,const char *units,char color){
    ymo_set_color_W();
    ymo_print_astr(label);
    ymo_set_color(color);
    ymo_print_uL_commas(value,0);
    ymo_print_astr(" ");
    ymo_print_astr(units);
    ymo_set_color_W();
    ymo_print_astr("\n");
}
void ymo_print_label_sF(const char *label,ym_sF value,const char *units,char color){
    ymo_set_color_W();
    ymo_print_astr(label);
    ymo_set_color(color);
    ymo_print_sF(value);
    ymo_print_astr(" ");
    ymo_print_astr(units);
    ymo_set_color_W();
    ymo_print_astr("\n");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char *YMO_BYTE_NAMES[] = {
    " bytes",
    " KB",
    " MB",
    " GB",
    " TB",
    " PB",
    " EB",
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline ym_pL ymo_print_u32(ym_u32 x){
    return ymo_print_u64(x);
}
inline ym_pL ymo_print_s32(ym_s32 x){
    return ymo_print_s64(x);
}
inline ym_pL ymo_print_u32_margin(ym_u32 x,ym_pL margin){
    return ymo_print_u64_margin(x,margin);
}
inline ym_pL ymo_print_s32_margin(ym_s32 x,ym_pL margin){
    return ymo_print_s64_margin(x,margin);
}
ym_pL ymo_print_u32_commas(ym_u32 x,ym_pL margin){
    return ymo_print_u64_commas(x,margin);
}
ym_pL ymo_print_s32_commas(ym_s32 x,ym_pL margin){
    return ymo_print_s64_commas(x,margin);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ym_pL ymo_print_u64(ym_u64 x){
    //Prints the decimal representation of x.
    //Returns the # of digits printed.

    //Status:
    // - Done

    if (x == 0){
        ymo_print_wstr(L"0");
        return 1;
    }

    int c = 21;
    wchar_t str[21];
    str[--c] = '\0';
    
    while (--c >= 0){
        str[c] = (char)(x % 10 + 48);
        x /= 10;
    }

    while (str[++c] == '0');
    ymo_print_wstr(str + c);
    return 20 - c;
}
ym_pL ymo_print_s64(ym_s64 x){
    ym_pL out = 0;
    if (x < 0){
        x = -x;
        ymo_print_wstr(L"-");
        out++;
    }
    return ymo_print_u64(x) + out;
}
ym_pL ymo_print_u64_margin(ym_u64 x,ym_pL margin){
    //  Prints the decimal representation of x.
    //  It will be aligned to the right in the specified margin size.

    //  Returns the # of characters that are printed.


    const ym_pL BUFFER_SIZE = 9 * 4 * 4/3 + 3;  //  Max string length
    wchar_t str[BUFFER_SIZE];                   //  Buffer for the string

    ym_pL out = 0;

    //  Large margin, print spaces.
    while (margin >= BUFFER_SIZE){
        ymo_print_wstr(L" ");
        out++;
        margin--;
    }

    ym_pL start = BUFFER_SIZE - 1 - margin;
    margin = BUFFER_SIZE - 1;
    str[margin] = '\0';

    //  Zero
    if (x == 0)
        str[--margin] = '0';

    //  Write the digits
    ym_pL c = 0;
    while (x != 0){
        ym_pL digit = (ym_pL)(x % 10);
        x /= 10;
        c++;
        str[--margin] = (char)digit + '0';
    }

#ifdef __INTEL_COMPILER
#pragma novector
#endif
    while (margin > start)
        str[--margin] = ' ';
    if (margin > start)
        margin = start;
    wchar_t *pstr = str + margin;
    ymo_print_wstr(pstr);

    while (*pstr != '\0'){
        out++;
        pstr++;
    }
    return out;
}
ym_pL ymo_print_s64_margin(ym_s64 x,ym_pL margin){
    //  Prints the decimal representation of x.
    //  It will be aligned to the right in the specified margin size.

    //  Returns the # of characters that are printed.


    const ym_pL BUFFER_SIZE = 9 * 4 * 4/3 + 3 + 1;  //  Max string length
    wchar_t str[BUFFER_SIZE];                       //  Buffer for the string

    ym_pL out = 0;

    //  Large margin, print spaces.
    while (margin >= BUFFER_SIZE){
        ymo_print_wstr(L" ");
        out++;
        margin--;
    }

    ym_pL start = BUFFER_SIZE - 1 - margin;
    margin = BUFFER_SIZE - 1;
    str[margin] = '\0';

    //  Zero
    if (x == 0)
        str[--margin] = '0';

    int negative = 0;
    if (x < 0){
        negative = 1;
        x = -x;
    }

    //  Write the digits
    ym_pL c = 0;
    while (x != 0){
        ym_pL digit = (ym_pL)(x % 10);
        x /= 10;
        c++;
        str[--margin] = (char)digit + '0';
    }

#ifdef __INTEL_COMPILER
#pragma novector
#endif
    if (negative && margin > start){
        str[--margin] = '-';
    }
    while (margin > start)
        str[--margin] = ' ';
    if (margin > start)
        margin = start;
    wchar_t *pstr = str + margin;
    ymo_print_wstr(pstr);

    while (*pstr != '\0'){
        out++;
        pstr++;
    }
    return out;
}
ym_pL ymo_print_u64_commas(ym_u64 x,ym_pL margin){
    //  Prints the decimal representation of x with comma separators.
    //  It will be aligned to the right in the specified margin size.

    //  Returns the # of characters that are printed.


    const ym_pL BUFFER_SIZE = 9 * 4 * 4/3 + 3;  //  Max string length
    wchar_t str[BUFFER_SIZE];                   //  Buffer for the string

    ym_pL out = 0;

    //  Large margin, print spaces.
    while (margin >= BUFFER_SIZE){
        ymo_print_wstr(L" ");
        out++;
        margin--;
    }

    ym_pL start = BUFFER_SIZE - 1 - margin;
    margin = BUFFER_SIZE - 1;
    str[margin] = '\0';

    //  Zero
    if (x == 0)
        str[--margin] = '0';

    //  Write the digits
    ym_pL c = 0;
    while (x != 0){
        ym_pL digit = (ym_pL)(x % 10);
        x /= 10;
        if (c == 3){    //  Write comma
            c = 0;
            str[--margin] = ',';
        }
        c++;
        str[--margin] = (char)digit + '0';
    }

#ifdef __INTEL_COMPILER
#pragma novector
#endif
    while (margin > start)
        str[--margin] = ' ';
    if (margin > start)
        margin = start;
    wchar_t *pstr = str + margin;
    ymo_print_wstr(pstr);

    while (*pstr != '\0'){
        out++;
        pstr++;
    }
    return out;
}
ym_pL ymo_print_s64_commas(ym_s64 x,ym_pL margin){
    ym_pL out = 0;
    if (x < 0){
        x = -x;
        ymo_print_astr("-");
        out++;
    }
    return ymo_print_u64_commas(x,margin) + out;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymo_print_chs(const unsigned char *A,ym_pL L){
    //Debugger

    if (L == 0){
        ymo_print_wstr(L"{}\n");
        return;
    }
    const unsigned char *stop = A + L - 1;
    ymo_print_wstr(L"{");
    while (A < stop){
        ymo_print_u32(*A++);
        ymo_print_wstr(L",");
    }
    ymo_print_u32(*A);
    ymo_print_wstr(L"}\n");
}
void ymo_print_u32s(const ym_u32 *A,ym_pL L){
    //Debugger

    if (L == 0){
        ymo_print_wstr(L"{}\n");
        return;
    }
    const ym_u32 *stop = A + L - 1;
    ymo_print_wstr(L"{");
    while (A < stop){
        ymo_print_u32(*A++);
        ymo_print_wstr(L",");
    }
    ymo_print_u32(*A);
    ymo_print_wstr(L"}\n");
}
void ymo_print_u32sr(const ym_u32 *A,ym_pL L){
    //Debugger

    if (L == 0){
        ymo_print_wstr(L"{}\n");
        return;
    }
    const ym_u32 *As = A + 1;
    A += L;
    ymo_print_wstr(L"{");
    while (A > As){
        ymo_print_u32(*--A);
        ymo_print_wstr(L",");
    }
    ymo_print_u32(*--A);
    ymo_print_wstr(L"}\n");
}
void ymo_print_s32s(const ym_s32 *A,ym_pL L){
    //Debugger

    if (L == 0){
        ymo_print_wstr(L"{}\n");
        return;
    }
    const ym_s32 *stop = A + L - 1;
    ymo_print_wstr(L"{");
    while (A < stop){
        ymo_print_s32(*A++);
        ymo_print_wstr(L",");
    }
    ymo_print_s32(*A);
    ymo_print_wstr(L"}\n");
}
void ymo_print_u64s(const ym_u64 *A,ym_pL L){
    //Debugger

    if (L == 0){
        ymo_print_wstr(L"{}\n");
        return;
    }
    const ym_u64 *stop = A + L - 1;
    ymo_print_wstr(L"{");
    while (A < stop){
        ymo_print_u64(*A++);
        ymo_print_wstr(L",");
    }
    ymo_print_u64(*A);
    ymo_print_wstr(L"}\n");
}
void ymo_print_u64sr(const ym_u64 *A,ym_pL L){
    //Debugger

    if (L == 0){
        ymo_print_wstr(L"{}\n");
        return;
    }
    const ym_u64 *As = A + 1;
    A += L;
    ymo_print_wstr(L"{");
    while (A > As){
        ymo_print_u64(*--A);
        ymo_print_wstr(L",");
    }
    ymo_print_u64(*--A);
    ymo_print_wstr(L"}\n");
}
void ymo_print_s64s(const ym_s64 *A,ym_pL L){
    //Debugger

    if (L == 0){
        ymo_print_wstr(L"{}\n");
        return;
    }
    const ym_s64 *stop = A + L - 1;
    ymo_print_wstr(L"{");
    while (A < stop){
        ymo_print_s64(*A++);
        ymo_print_wstr(L",");
    }
    ymo_print_s64(*A);
    ymo_print_wstr(L"}\n");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymo_error(const char *msg){
    ymo_print_astr("\n\n");
    ymo_set_color_R(1);
    ymo_print_astr(msg);
    ymo_set_color_W();
    ymo_print_astr("\n\n");
#ifdef YCC_ENABLE_CRASH_DEBUGGING
    ym_pause();
    int *p = NULL;
    *p = 0;
#endif
    ym_quit(1);
}
void ymo_error_wstr(const wchar_t *msg){
    ymo_print_wstr(L"\n\n");
    ymo_set_color_R(1);
    ymo_print_wstr(msg);
    ymo_set_color_W();
    ymo_print_wstr(L"\n\n");
#ifdef YCC_ENABLE_CRASH_DEBUGGING
    ym_pause();
    int *p = NULL;
    *p = 0;
#endif
    ym_quit(1);
}
void ymo_warning(const char *msg){
    ymo_print_astr("\n");
    ymo_set_color_R(1);
    ymo_print_astr(msg);
    ymo_set_color_W();
}
void ymo_warning_wstr(const wchar_t *msg){
    ymo_print_wstr(L"\n");
    ymo_set_color_R(1);
    ymo_print_wstr(msg);
    ymo_set_color_W();
}
void ymo_throw_limit(ym_uL value,ym_uL limit,const wchar_t *msg){
    ymo_set_color_R(1);
    ymo_print_astr("\n\n");
    ymo_print_wstr(msg);
    ymo_print_astr("\n\n");
    ymo_print_astr("Value = "); ymo_print_uL(value); ymo_print_astr("\n");
    ymo_print_astr("Limit = "); ymo_print_uL(limit); ymo_print_astr("\n");
    ymo_set_color_W();

    ym_pause();
    int *p = NULL;
    ym_quit(*p);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymo_print_bytes(ym_uL bytes,int round){
    //  Prints out bytes in one of the following forms:
    //  0.xx suffix
    //  x.xx suffix
    //  xx.x suffix
    //   xxx suffix

    //  round = 0   Round down.
    //  round = 1   Round up.
    //  round = -1  Round up only if close to a small multiple of a power of two.


    //  Less than 1 KB.
    if (bytes < 1024){
        if (bytes < 1000){
            ymo_print_u32((ym_u32)bytes);
            ymo_print_astr(YMO_BYTE_NAMES[0]);
        }else{
            bytes *= 100;
            if (round == 1)
                bytes += 1023;
            bytes >>= 10;
            ymo_print_astr("0.");
            ymo_print_u32((ym_u32)bytes);
            ymo_print_astr(YMO_BYTE_NAMES[1]);
        }
        return;
    }


    int shift = 0;
    ym_uL factor = 1;

    //  Avoid overflow.
    if (bytes > ((ym_uL)0 - 1) / 1000){
        if (round == 1 && (bytes & (ym_uL)1023)){
            bytes >>= 10;
            bytes++;
        }else{
            bytes >>= 10;
        }
        shift++;
    }


    //  Round it up if it is close to a small multiple of a power of two.
    if (round == -1){
        //  Find a chain of ones.
        ym_pL c = 0;
        ym_pL shift = YM_IND_BITS;
        ym_uL t = bytes;
        ym_uL MASK = (ym_uL)1 << (YM_IND_BITS - 1);
        while (shift > 0){
            if ((t & MASK) == 0){
                if (c >= 6)
                    break;
                c = 0;
            }else{
                c++;
            }
            t <<= 1;
            shift--;
        }
        if (c >= 6){
            t = (ym_uL)1 << shift;
            bytes += t;
            bytes &= ~(t - 1);
        }
    }



    //  Shift up to the right suffix.
    do{
        factor <<= 10;
        shift++;
    }while (bytes >= 1000*factor);

    //  Multiply by 1000 to extract 3 decimal places.
    bytes *= 1000;

    //  Divide by factor.
    if (round == 1)
        bytes += factor - 1;
    bytes >>= 10*shift;

    //  .000
    if (bytes < 1000){
        ymo_print_astr("0.");
        ymo_print_u32(round == 1 ? ((ym_u32)bytes + 9) / 10 : (ym_u32)bytes / 10);
        ymo_print_astr(YMO_BYTE_NAMES[shift]);
        return;
    }

    if (round == 1)
        bytes += 9;
    bytes /= 10;

    //  0.00
    if (bytes < 1000){
        ymo_print_u32((ym_u32)bytes / 100);
        ymo_print_astr(".");
        bytes %= 100;
        if (bytes >= 10)
            ymo_print_u32((ym_u32)bytes);
        else{
            ymo_print_astr("0");
            ymo_print_u32((ym_u32)bytes);
        }
        ymo_print_astr(YMO_BYTE_NAMES[shift]);
        return;
    }

    if (round == 1)
        bytes += 9;
    bytes /= 10;

    //  00.0
    if (bytes < 1000){
        ymo_print_u32((ym_u32)bytes / 10);
        ymo_print_astr(".");
        ymo_print_u32((ym_u32)bytes % 10);
        ymo_print_astr(YMO_BYTE_NAMES[shift]);
        return;
    }

    if (round == 1)
        bytes += 9;
    bytes /= 10;

    //  000.
    if (bytes < 1000){
        ymo_print_astr(" ");
        ymo_print_u32((ym_u32)bytes);
        ymo_print_astr(YMO_BYTE_NAMES[shift]);
        return;
    }

    ymo_print_astr("0.98");
    ymo_print_astr(YMO_BYTE_NAMES[shift + 1]);
}
////////////////////////////////////////////////////////////////////////////////
ym_uL ymo_prompt_uL(const wchar_t *msg,ym_uL low_limit,ym_uL high_limit){
    ym_uL x = 0;
    do{
        x = 0;
        ymo_print_wstr(msg);
        ymo_set_color_T(1);
        wchar_t ch;
        while (1){
            ch = fgetwc(stdin);
            if (ch < L'0' || ch > L'9')
                break;
            x *= 10;
            x += ch - L'0';
        }
//        cin >> x;
        ymo_set_color_W();
    }while (x < low_limit || x > high_limit);
//    ymo_print_wstr(L"\n");
    return x;
}
ym_uL ymo_prompt_bytes(){
    wchar_t buffer[256];
    ymo_scan_str(buffer,256);

    ym_uL x = 0;
    ym_pL c = 0;
    wchar_t ch = buffer[c++];
    while (ch >= L'0' && ch <= L'9'){
        x *= 10;
        x += ch - L'0';
        ch = buffer[c++];
    }

    double frac = 0;
    if (ch == '.'){
        double f = 0.1;

        ch = buffer[c++];
        while (ch >= L'0' && ch <= L'9'){
            frac += f * (ch - L'0');
            f *= 0.1;
            ch = buffer[c++];
        }
    }

    while (ch == ' ')
        ch = buffer[c++];

    switch (ch){
        case L'k':
        case L'K':
            return (x << 10) + (ym_uL)ceil(frac * ((ym_uL)1 << 10));
        case L'm':
        case L'M':
            return (x << 20) + (ym_uL)ceil(frac * ((ym_uL)1 << 20));
        case L'g':
        case L'G':
            return (x << 30) + (ym_uL)ceil(frac * ((ym_uL)1 << 30));
        case L't':
        case L'T':
            return (x << 40) + (ym_uL)ceil(frac * ((ym_uL)1 << 40));
        case L'p':
        case L'P':
            return (x << 50) + (ym_uL)ceil(frac * ((ym_uL)1 << 50));
        case L'e':
        case L'E':
            return (x << 60) + (ym_uL)ceil(frac * ((ym_uL)1 << 60));
        default:
            return x;
    };
}
ym_kL ymo_prompt_thread_mode(){
    ymo_print_astr("\n");
    ym_kL out = (ym_kL)ymo_prompt_uL(L"k = ",0,YMS_MAX_THREAD_K);
    return out;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif