/* ymb_APIs_StringBuilder_internals.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 03/08/2009
 * Last Modified    : 03/08/2009
 * 
 */

#pragma once
#ifndef _ymb_APIs_StringBuilder_internals_H
#define _ymb_APIs_StringBuilder_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "ymb_APIs_StringBuilder_headers.h"
#include "../ConsoleIO/ymo_APIs_cio_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ym_sba_init(ym_heap *ah,ym_sba *T){
    //  Initializes a new string builder.

    T->len = 0;
    T->alen = 16;
    T->buffer = (char*)ym_ah_malloc(ah,T->alen * sizeof(char),sizeof(char));
    if (T->buffer == NULL)
        ym_allocfail();
}
void ym_sba_kill(ym_sba *T){
    //  Frees a string builder.

    ym_ah_free(T->buffer);
}
void ym_sba_clear(ym_sba *T){
    //  Clears the string builder by removing all characters currently stored in it.

    T->len = 0;
}
char* ym_sba_transfer(ym_sba *T){
    //  Returns the C-string for this string builder.
    //  The returned pointer needs to be freed manually.
    //  The string builder is uninitialized after this call.

    T->len = 0;
    T->alen = 0;
    char *out = T->buffer;
    T->buffer = NULL;
    return out;
}
void ym_sba_push_char(ym_heap *ah,ym_sba *T,char ch){
    //  Append a char to this string builder.

    if (T->len == T->alen){
        T->alen <<= 1;

        char *V = (char*)ym_ah_malloc(ah,T->alen * sizeof(char),sizeof(char));
        if (V == NULL)
            ym_allocfail();

        ym_pL c = 0;

#ifdef __INTEL_COMPILER
#pragma novector
#endif
        while (c < T->len){
            V[c] = T->buffer[c];
            c++;
        }

        ym_ah_free(T->buffer);
        T->buffer = V;
    }

    T->buffer[T->len++] = ch;
}
void ym_sba_push_str(ym_heap *ah,ym_sba *T,const char *str){
    //  Append an ASCII string.

    while (*str != '\0')
        ym_sba_push_char(ah,T,(char)*str++);
}
void ym_sba_push_wstr(ym_heap *ah,ym_sba *T,const wchar_t *str){
    //  Append an ASCII string.

    while (*str != '\0')
        ym_sba_push_char(ah,T,(char)*str++);
}
////////////////////////////////////////////////////////////////////////////////
void ym_sba_push_uL(ym_heap *ah,ym_sba *T,ym_uL x){
#if YM_INDEX_MAG <= 5
    const ym_pL BUFFER_SIZE = 11;
#elif YM_INDEX_MAG == 6
    const ym_pL BUFFER_SIZE = 22;
#else
#error "YM_INDEX_MAG > 6"
#endif

    if (x == 0){
        ym_sba_push_char(ah,T,'0');
        return;
    }

    ym_pL L = BUFFER_SIZE;

    char str[BUFFER_SIZE];
    str[--L] = '\0';

    ym_pL c = L;
    while (x > 0){
        str[--c] = (x % 10) + '0';
        x /= 10;
    }

    ym_sba_push_str(ah,T,str + c);
}
void ym_sba_push_sL(ym_heap *ah,ym_sba *T,ym_sL x){
    if (x < 0){
        ym_sba_push_char(ah,T,L'-');
        x = -x;
    }
    ym_sba_push_uL(ah,T,x);
}
void ym_sba_push_uL_commas(ym_heap *ah,ym_sba *T,ym_uL x){
#if YM_INDEX_MAG <= 5
    const ym_pL BUFFER_SIZE = 11 + 3;
#elif YM_INDEX_MAG == 6
    const ym_pL BUFFER_SIZE = 22 + 6;
#else
#error "YM_INDEX_MAG > 6"
#endif

    if (x == 0){
        ym_sba_push_char(ah,T,'0');
        return;
    }

    ym_pL L = BUFFER_SIZE;

    char str[BUFFER_SIZE];
    str[--L] = '\0';

    ym_pL c = 0;
    while (x > 0){
        ym_pL digit = (ym_pL)(x % 10);
        x /= 10;
        if (c == 3){
            c = 0;
            str[--L] = ',';
        }
        c++;
        str[--L] = (char)digit + '0';
    }

    ym_sba_push_str(ah,T,str + L);
}
void ym_sba_push_sFd3(ym_heap *ah,ym_sba *T,ym_sF x){
    if (x < 0){
        ym_sba_push_char(ah,T,'-');
        ym_sba_push_sFd3(ah,T,-x);
        return;
    }

    ym_uL int_part = (ym_uL)x;
    x -= int_part;
    x *= 1000;
    ym_uL fract_part = (ym_uL)x;
    if (fract_part > 999)
        fract_part = 999;

    ym_sba_push_uL(ah,T,int_part);
    ym_sba_push_char(ah,T,'.');
    if (x >= 100){
        ym_sba_push_uL(ah,T,fract_part);
    }else if (x >= 10){
        ym_sba_push_char(ah,T,'0');
        ym_sba_push_uL(ah,T,fract_part);
    }else if (x >= 1){
        ym_sba_push_str(ah,T,"00");
        ym_sba_push_uL(ah,T,fract_part);
    }else{
        ym_sba_push_str(ah,T,"000");
    }
}
////////////////////////////////////////////////////////////////////////////////
void ym_sba_push_bytes(ym_heap *ah,ym_sba *T,const char *data,ym_pL bytes){
    while (bytes-- > 0)
        ym_sba_push_char(ah,T,(char)*data++);
}
void ym_sba_push_size_bytes(ym_heap *ah,ym_sba *T,ym_uL bytes,int round){
    if (bytes < 1024){
        if (bytes < 1000){
            ym_sba_push_uL(ah,T,bytes);
            ym_sba_push_str(ah,T,YMO_BYTE_NAMES[0]);
//            ymo_print_astr(YMO_BYTE_NAMES[0]);
        }else{
            bytes *= 100;
            bytes >>= 10;
            ym_sba_push_str(ah,T,"0.");
            ym_sba_push_uL(ah,T,bytes);
            ym_sba_push_str(ah,T,YMO_BYTE_NAMES[1]);
        }
        return;
    }


    //  Round it up if it is close to a small multiple of a power of two.
    if (round){
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


    int c = 1;
    while (bytes >= 1048576){
        bytes >>= 10;
        c++;
    }

    if (bytes >= 1024 * 1000){
        bytes >>= 10;
        bytes *= 100;
        bytes >>= 10;
        ym_sba_push_str(ah,T,"0.");
        ym_sba_push_uL(ah,T,bytes);
        ym_sba_push_str(ah,T,YMO_BYTE_NAMES[c + 1]);
    }else if (bytes >= 1024 * 100){
        bytes >>= 10;
//        ym_sba_push_str(ah,T," ");
        ym_sba_push_uL(ah,T,bytes);
        ym_sba_push_str(ah,T,YMO_BYTE_NAMES[c]);
    }else if (bytes >= 1024 * 10){
        bytes *= 10;
        bytes >>= 10;
        ym_sba_push_uL(ah,T,bytes / 10);
        ym_sba_push_str(ah,T,".");
        ym_sba_push_uL(ah,T,bytes % 10);
        ym_sba_push_str(ah,T,YMO_BYTE_NAMES[c]);
    }else{
        bytes *= 100;
        bytes >>= 10;
        ym_sba_push_uL(ah,T,bytes / 100);
        ym_sba_push_str(ah,T,".");
        bytes %= 100;
        if (bytes >= 10)
            ym_sba_push_uL(ah,T,bytes);
        else{
            ym_sba_push_str(ah,T,"0");
            ym_sba_push_uL(ah,T,bytes);
        }
       ym_sba_push_str(ah,T,YMO_BYTE_NAMES[c]);
    }
}
const char* ym_sba_str_delim(ym_heap *ah,ym_sba *T,const char *str,char delim){
    //  Streams characters from "str" into the builder until a deliminator is found.

    char ch;
    do{
        ch = *str++;
        if (ch == '\r')
            continue;
        if (ch == delim || ch == '\0')
            return str;
        ym_sba_push_char(ah,T,ch);
    }while (1);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Wide Character
void ym_sbw_init(ym_heap *ah,ym_sbw *T){
    //  Initializes a new string builder.

    T->len = 0;
    T->alen = 16;
    T->buffer = (wchar_t*)ym_ah_malloc(ah,T->alen * sizeof(wchar_t),sizeof(wchar_t));
    if (T->buffer == NULL)
        ym_allocfail();
}
void ym_sbw_kill(ym_sbw *T){
    //  Frees a string builder.

    ym_ah_free(T->buffer);
}
void ym_sbw_clear(ym_sbw *T){
    //  Clears the string builder by removing all characters currently stored in it.

    T->len = 0;
}
wchar_t* ym_sbw_transfer(ym_sbw *T){
    //  Returns the C-string for this string builder.
    //  The returned pointer needs to be freed manually.
    //  The string builder is uninitialized after this call.

    T->len = 0;
    T->alen = 0;
    wchar_t *out = T->buffer;
    T->buffer = NULL;
    return out;
}
void ym_sbw_push_char(ym_heap *ah,ym_sbw *T,wchar_t ch){
    //  Append a char to this string builder.

    if (T->len == T->alen){
        T->alen <<= 1;
        wchar_t *V = (wchar_t*)ym_ah_malloc(ah,T->alen * sizeof(wchar_t),sizeof(wchar_t));
//        if (V == NULL){
//            ym_allocfail();
//        }

        ym_pL c = 0;
        
#ifdef __INTEL_COMPILER
#pragma novector
#endif
        while (c < T->len){
            V[c] = T->buffer[c];
            c++;
        }

        ym_ah_free(T->buffer);
        T->buffer = V;
    }

    T->buffer[T->len++] = ch;
}
void ym_sbw_push_astr(ym_heap *ah,ym_sbw *T,const char *str){
    //  Append an ASCII string.

    while (*str != '\0')
        ym_sbw_push_char(ah,T,(wchar_t)*str++);
}
void ym_sbw_push_wstr(ym_heap *ah,ym_sbw *T,const wchar_t *str){
    //  Append a unicode string.

    while (*str != L'\0')
        ym_sbw_push_char(ah,T,*str++);
}
////////////////////////////////////////////////////////////////////////////////
void ym_sbw_push_uL(ym_heap *ah,ym_sbw *T,ym_uL x){
#if YM_INDEX_MAG <= 5
    const ym_pL BUFFER_SIZE = 11;
#elif YM_INDEX_MAG == 6
    const ym_pL BUFFER_SIZE = 22;
#else
#error "YM_INDEX_MAG > 6"
#endif

    if (x == 0){
        ym_sbw_push_char(ah,T,'0');
        return;
    }

    ym_pL L = BUFFER_SIZE;

    char str[BUFFER_SIZE];
    str[--L] = '\0';

    ym_pL c = L;
    while (x > 0){
        str[--c] = (x % 10) + '0';
        x /= 10;
    }

    ym_sbw_push_astr(ah,T,str + c);
}
void ym_sbw_push_sL(ym_heap *ah,ym_sbw *T,ym_sL x){
    if (x < 0){
        ym_sbw_push_char(ah,T,L'-');
        x = -x;
    }
    ym_sbw_push_uL(ah,T,x);
}
void ym_sbw_push_uL_commas(ym_heap *ah,ym_sbw *T,ym_uL x){
#if YM_INDEX_MAG <= 5
    const ym_pL BUFFER_SIZE = 11 + 3;
#elif YM_INDEX_MAG == 6
    const ym_pL BUFFER_SIZE = 22 + 6;
#else
#error "YM_INDEX_MAG > 6"
#endif

    if (x == 0){
        ym_sbw_push_char(ah,T,'0');
        return;
    }

    ym_pL L = BUFFER_SIZE;

    char str[BUFFER_SIZE];
    str[--L] = '\0';

    ym_pL c = 0;
    while (x > 0){
        ym_pL digit = (ym_pL)(x % 10);
        x /= 10;
        if (c == 3){
            c = 0;
            str[--L] = ',';
        }
        c++;
        str[--L] = (char)digit + '0';
    }

    ym_sbw_push_astr(ah,T,str + L);
}
void ym_sbw_push_sFd3(ym_heap *ah,ym_sbw *T,ym_sF x){
    if (x < 0){
        ym_sbw_push_char(ah,T,'-');
        ym_sbw_push_sFd3(ah,T,-x);
        return;
    }

    ym_uL int_part = (ym_uL)x;
    x -= int_part;
    x *= 1000;
    ym_uL fract_part = (ym_uL)x;
    if (fract_part > 999)
        fract_part = 999;

    ym_sbw_push_uL(ah,T,int_part);
    ym_sbw_push_char(ah,T,'.');
    if (x >= 100){
        ym_sbw_push_uL(ah,T,fract_part);
    }else if (x >= 10){
        ym_sbw_push_char(ah,T,'0');
        ym_sbw_push_uL(ah,T,fract_part);
    }else if (x >= 1){
        ym_sbw_push_astr(ah,T,"00");
        ym_sbw_push_uL(ah,T,fract_part);
    }else{
        ym_sbw_push_astr(ah,T,"000");
    }
}
void ym_sbw_push_sF(ym_heap *ah,ym_sbw *T,ym_sF x,ym_pL digits){
    if (x < 0){
        ym_sbw_push_char(ah,T,'-');
        ym_sbw_push_sF(ah,T,-x,digits);
        return;
    }

    ym_uL int_part = (ym_uL)x;
    x -= int_part;

    ym_sbw_push_uL(ah,T,int_part);
    ym_sbw_push_char(ah,T,'.');

    //  Round it
    ym_uL scale = 1;
    for (ym_pL c = 0; c < digits; c++){
        scale *= 10;
    }
    x += 0.5 / scale;

    while (digits-- > 0){
        x *= 10;
        int digit = (int)x;
        if (digit > 9)
            digit = 9;
        ym_sbw_push_char(ah,T,(char)(digit + '0'));
        x -= digit;
    }
}
////////////////////////////////////////////////////////////////////////////////
void ym_sbw_push_size_bytes(ym_heap *ah,ym_sbw *T,ym_uL bytes,int round){
    if (bytes < 1024){
        if (bytes < 1000){
            ym_sbw_push_uL(ah,T,bytes);
            ym_sbw_push_astr(ah,T,YMO_BYTE_NAMES[0]);
//            ymo_print_astr(YMO_BYTE_NAMES[0]);
        }else{
            bytes *= 100;
            bytes >>= 10;
            ym_sbw_push_astr(ah,T,"0.");
            ym_sbw_push_uL(ah,T,bytes);
            ym_sbw_push_astr(ah,T,YMO_BYTE_NAMES[1]);
        }
        return;
    }

    //  Round it up if it is close to a small multiple of a power of two.
    if (round){
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


    int c = 1;
    while (bytes >= 1048576){
        bytes >>= 10;
        c++;
    }

    if (bytes >= 1024 * 1000){
        bytes >>= 10;
        bytes *= 100;
        bytes >>= 10;
        ym_sbw_push_astr(ah,T,"0.");
        ym_sbw_push_uL(ah,T,bytes);
        ym_sbw_push_astr(ah,T,YMO_BYTE_NAMES[c + 1]);
    }else if (bytes >= 1024 * 100){
        bytes >>= 10;
//        ym_sba_push_str(ah,T," ");
        ym_sbw_push_uL(ah,T,bytes);
        ym_sbw_push_astr(ah,T,YMO_BYTE_NAMES[c]);
    }else if (bytes >= 1024 * 10){
        bytes *= 10;
        bytes >>= 10;
        ym_sbw_push_uL(ah,T,bytes / 10);
        ym_sbw_push_astr(ah,T,".");
        ym_sbw_push_uL(ah,T,bytes % 10);
        ym_sbw_push_astr(ah,T,YMO_BYTE_NAMES[c]);
    }else{
        bytes *= 100;
        bytes >>= 10;
        ym_sbw_push_uL(ah,T,bytes / 100);
        ym_sbw_push_astr(ah,T,".");
        bytes %= 100;
        if (bytes >= 10)
            ym_sbw_push_uL(ah,T,bytes);
        else{
            ym_sbw_push_astr(ah,T,"0");
            ym_sbw_push_uL(ah,T,bytes);
        }
       ym_sbw_push_astr(ah,T,YMO_BYTE_NAMES[c]);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
