/* ymb_APIs_StringTools_internals.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/20/2011
 * Last Modified    : 04/20/2011
 * 
 * 
 *      ymb         =   Basic Function
 *      APIs        =   APIs
 *      StringTools =   String Tools
 *      internals   =   Module Internals
 * 
 */

#pragma once
#ifndef _ymb_APIs_StringTools_internals_H
#define _ymb_APIs_StringTools_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string.h>
#include "ymb_APIs_StringTools_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ym_pL ymb_str_a_len(const char *A){
    //  Returns the length excluding the NULL.

    ym_pL L = 0;
    while (A[L] != '\0')
        L++;
    return L;
}
ym_pL ymb_str_w_len(const wchar_t *A){
    //  Returns the length excluding the NULL.

    ym_pL L = 0;
    while (A[L] != L'\0')
        L++;
    return L;
}
char* ymb_str_a_copy_n(ym_heap *ah,const char *A){
    ym_pL L = ymb_str_a_len(A) + 1;

    char *out = ym_ah_malloc_ch(ah,L);
    if (out == NULL)
        return NULL;

    ym_pL c = 0;

#ifdef __INTEL_COMPILER
#pragma novector
#endif
    while (c < L)
        out[c++] = *A++;

    return out;
}
char* ymb_str_a_copy_t(ym_heap *ah,const char *A){
    char *out = ymb_str_a_copy_n(ah,A);
    if (out == NULL)
        ym_allocfail();
    return out;
}
wchar_t* ymb_str_w_copy_n(ym_heap *ah,const wchar_t *A){
    ym_pL L = ymb_str_w_len(A) + 1;

    wchar_t *out = (wchar_t*)ym_ah_malloc(ah,L * sizeof(wchar_t),sizeof(wchar_t));
    if (out == NULL)
        return NULL;

    ym_pL c = 0;

#ifdef __INTEL_COMPILER
#pragma novector
#endif
    while (c < L)
        out[c++] = *A++;

    return out;
}
wchar_t* ymb_str_w_copy_t(ym_heap *ah,const wchar_t *A){
    wchar_t *out = ymb_str_w_copy_n(ah,A);
    if (out == NULL)
        ym_allocfail();
    return out;
}
wchar_t* ymb_str_aw_copy_n(ym_heap *ah,const char *A){
    ym_pL L = ymb_str_a_len(A) + 1;

    wchar_t *out = ym_ah_malloc_wch(ah,L);
    if (out == NULL)
        return NULL;

    ym_pL c = 0;

#ifdef __INTEL_COMPILER
#pragma novector
#endif
    while (c < L)
        out[c++] = (wchar_t)*A++;

    return out;
}
wchar_t* ymb_str_aw_copy_t(ym_heap *ah,const char *A){
    wchar_t *out = ymb_str_aw_copy_n(ah,A);
    if (out == NULL)
        ym_allocfail();
    return out;
}
char* ymb_str_wa_copy_n(ym_heap *ah,const wchar_t *A){
    ym_pL L = ymb_str_w_len(A) + 1;

    char *out = ym_ah_malloc_ch(ah,L);
    if (out == NULL)
        return NULL;

    ym_pL c = 0;

#ifdef __INTEL_COMPILER
#pragma novector
#endif
    while (c < L)
        out[c++] = (char)*A++;

    return out;
}
char* ymb_str_wa_copy_t(ym_heap *ah,const wchar_t *A){
    char *out = ymb_str_wa_copy_n(ah,A);
    if (out == NULL)
        ym_allocfail();
    return out;
}
wchar_t* ymb_str_ww_append_n(ym_heap *ah,const wchar_t *A,const wchar_t *B){
    ym_pL AL = ymb_str_w_len(A);
    ym_pL BL = ymb_str_w_len(B);
    ym_pL allocL = AL + BL + 1;

    wchar_t *out = (wchar_t*)ym_ah_malloc(ah,allocL * sizeof(wchar_t),sizeof(wchar_t));
    if (out == NULL)
        return NULL;

    ym_pL c = 0;
    while (c < AL)
        out[c++] = *A++;
    BL += c;
    while (c < BL)
        out[c++] = *B++;
    out[c] = L'\0';

    return out;
}
wchar_t* ymb_str_ww_append_t(ym_heap *ah,const wchar_t *A,const wchar_t *B){
    wchar_t *out = ymb_str_ww_append_n(ah,A,B);
    if (out == NULL)
        ym_allocfail();
    return out;
}
int ymb_str_a_eq(const char *a,const char *b){
    if (a == b)
        return 1;
    while (*a == *b){
        if (*a == '\0')
            return 1;
        a++;
        b++;
    }
    return 0;
}
int ymb_str_w_eq(const wchar_t *a,const wchar_t *b){
    if (a == b)
        return 1;
    while (*a == *b){
        if (*a == L'\0')
            return 1;
        a++;
        b++;
    }
    return 0;
}
wchar_t* ymb_str_build_path_(ym_heap *ah,const wchar_t *path,const wchar_t *name){
    const wchar_t tmp[] = L"";
    if (path == NULL)
        path = tmp;
    ym_pL pL = ymb_str_w_len(path);
    ym_pL nL = ymb_str_w_len(name);

    ym_pL oL = pL + nL + 2;
    wchar_t *newpath = (wchar_t*)ym_ah_malloc(ah,oL * sizeof(wchar_t),0);

    ym_pL c = 0;
    if (pL > 0){
        memcpy(newpath,path,pL * sizeof(wchar_t));
        c += pL;
        if (path[pL - 1] != '/' && path[pL - 1] != '\\')
            newpath[c++] = '/';
    }
    memcpy(newpath + c,name,nL * sizeof(wchar_t));
    c += nL;
    newpath[c] = L'\0';

    return newpath;
}
const char* ymb_str_a_parse_sL(const char *str,ym_sL *x){
    ym_sL x_ = 0;
    char ch;
    while (1){
        ch = *str;
        if (ch < '0' || ch > '9')
            break;
        x_ *= 10;
        x_ += ch - '0';
        str++;
    }

    x[0] = x_;

    return str;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
