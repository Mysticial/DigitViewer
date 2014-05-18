/* ymb_APIs_StringBuilder_headers.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 03/08/2009
 * Last Modified    : 03/08/2009
 * 
 * 
 *      This file has been depreciated since y-cruncher v0.6.2.
 *      Just use C++ "std::string" and "std::wstring" instead.
 */

#pragma once
#ifndef _ymb_APIs_StringBuilder_headers_H
#define _ymb_APIs_StringBuilder_headers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <stdio.h>
#ifndef _ym_SelfContainedHeader_H
#include "../Memory/ymi_APIs_ah_headers.h"
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  ASCII
typedef struct{
    char *buffer;
    ym_pL len;
    ym_pL alen;
} ym_sba;
extern  void        ym_sba_init             (ym_heap *ah,ym_sba *T);
extern  void        ym_sba_kill             (ym_sba *T);
extern  void        ym_sba_clear            (ym_sba *T);
extern  char*       ym_sba_transfer         (ym_sba *T);
extern  void        ym_sba_push_char        (ym_heap *ah,ym_sba *T,char ch);
extern  void        ym_sba_push_str         (ym_heap *ah,ym_sba *T,const char *str);
extern  void        ym_sba_push_wstr        (ym_heap *ah,ym_sba *T,const wchar_t *str);
extern  void        ym_sba_push_uL          (ym_heap *ah,ym_sba *T,ym_uL x);
extern  void        ym_sba_push_sL          (ym_heap *ah,ym_sba *T,ym_sL x);
extern  void        ym_sba_push_uL_commas   (ym_heap *ah,ym_sba *T,ym_uL x);
extern  void        ym_sba_push_sFd3        (ym_heap *ah,ym_sba *T,ym_sF x);
extern  void        ym_sba_push_bytes       (ym_heap *ah,ym_sba *T,const char *data,ym_pL bytes);
extern  void        ym_sba_push_size_bytes  (ym_heap *ah,ym_sba *T,ym_uL bytes,int round);
extern  const char* ym_sba_str_delim        (ym_heap *ah,ym_sba *T,const char *str,char delim);
extern  int         ym_sba_file_delim       (ym_heap *ah,ym_sba *T,FILE *file,char delim);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Unicode
typedef struct{
    wchar_t *buffer;
    ym_pL len;
    ym_pL alen;
} ym_sbw;
extern  void        ym_sbw_init             (ym_heap *ah,ym_sbw *T);
extern  void        ym_sbw_kill             (ym_sbw *T);
extern  void        ym_sbw_clear            (ym_sbw *T);
extern  wchar_t*    ym_sbw_transfer         (ym_sbw *T);
extern  void        ym_sbw_push_char        (ym_heap *ah,ym_sbw *T,wchar_t ch);
extern  void        ym_sbw_push_astr        (ym_heap *ah,ym_sbw *T,const char *str);
extern  void        ym_sbw_push_wstr        (ym_heap *ah,ym_sbw *T,const wchar_t *str);
extern  void        ym_sbw_push_uL          (ym_heap *ah,ym_sbw *T,ym_uL x);
extern  void        ym_sbw_push_sL          (ym_heap *ah,ym_sbw *T,ym_sL x);
extern  void        ym_sbw_push_uL_commas   (ym_heap *ah,ym_sbw *T,ym_uL x);
extern  void        ym_sbw_push_sFd3        (ym_heap *ah,ym_sbw *T,ym_sF x);
extern  void        ym_sbw_push_sF          (ym_heap *ah,ym_sbw *T,ym_sF x,ym_pL digits);
extern  void        ym_sbw_push_size_bytes  (ym_heap *ah,ym_sbw *T,ym_uL bytes,int round);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
#endif
