/* ymb_APIs_StringTools_headers.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/20/2011
 * Last Modified    : 04/20/2011
 * 
 * 
 *      ymb         =   Basic Function
 *      APIs        =   APIs
 *      StringTools =   String Tools
 *      headers     =   Module Headers
 * 
 * 
 *      This file has been depreciated since y-cruncher v0.6.2.
 *      Just use C++ "std::string" and "std::wstring" instead.
 */

#pragma once
#ifndef _ymb_APIs_StringTools_headers_H
#define _ymb_APIs_StringTools_headers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
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
extern  ym_pL           ymb_str_a_len           (const char *A);
extern  ym_pL           ymb_str_w_len           (const wchar_t *A);
extern  char*           ymb_str_a_copy_n        (ym_heap *ah,const char *A);
extern  char*           ymb_str_a_copy_t        (ym_heap *ah,const char *A);
extern  wchar_t*        ymb_str_w_copy_n        (ym_heap *ah,const wchar_t *A);
extern  wchar_t*        ymb_str_w_copy_t        (ym_heap *ah,const wchar_t *A);
extern  wchar_t*        ymb_str_aw_copy_n       (ym_heap *ah,const char *A);
extern  wchar_t*        ymb_str_aw_copy_t       (ym_heap *ah,const char *A);
extern  char*           ymb_str_wa_copy_n       (ym_heap *ah,const wchar_t *A);
extern  char*           ymb_str_wa_copy_t       (ym_heap *ah,const wchar_t *A);
extern  wchar_t*        ymb_str_ww_append_n     (ym_heap *ah,const wchar_t *A,const wchar_t *B);
extern  wchar_t*        ymb_str_ww_append_t     (ym_heap *ah,const wchar_t *A,const wchar_t *B);
extern  int             ymb_str_a_eq            (const char *a,const char *b);
extern  int             ymb_str_w_eq            (const wchar_t *a,const wchar_t *b);
//extern  wchar_t*        ymb_str_build_path      (ym_heap *ah,const wchar_t *path,const wchar_t *name);
extern  const char*     ymb_str_a_parse_sL      (const char *str,ym_sL *x);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
#endif
