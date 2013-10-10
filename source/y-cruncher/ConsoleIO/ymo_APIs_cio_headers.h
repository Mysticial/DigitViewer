/* ymo_APIs_cio_headers.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/20/2011
 * Last Modified    : 04/20/2011
 * 
 * 
 *      ymo     =   I/O Function
 *      APIs    =   APIs
 *      cio     =   Console I/O
 *      headers =   Module Headers
 * 
 */

#pragma once
#ifndef _ymi_APIs_cio_headers_H
#define _ymi_APIs_cio_headers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#ifndef _ym_SelfContainedHeader_H
#include "../../Globals.h"
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
extern  void    yma_APIs_cio            ();
////////////////////////////////////////////////////////////////////////////////
//  Basic Printing
extern  void    ymo_print_astr          (const char *str);
extern  void    ymo_print_wstr          (const wchar_t *str);
extern  void    ymo_scan_str            (wchar_t *buffer,ym_pL L);
////////////////////////////////////////////////////////////////////////////////
//  Colors
extern  void    ymo_set_color           (char color);
extern  void    ymo_set_color_W         ();
extern  void    ymo_set_color_R         (int intense);
extern  void    ymo_set_color_Y         (int intense);
extern  void    ymo_set_color_G         (int intense);
extern  void    ymo_set_color_B         (int intense);
extern  void    ymo_set_color_T         (int intense);
extern  void    ymo_set_color_P         (int intense);
extern  void    ymo_print_label         (const char *label,const char *value,char color);
extern  void    ymo_print_label_uL          (const char *label,ym_uL value,const char *units,char color);
extern  void    ymo_print_label_uL_commas   (const char *label,ym_uL value,const char *units,char color);
extern  void    ymo_print_label_sF          (const char *label,ym_sF value,const char *units,char color);
////////////////////////////////////////////////////////////////////////////////
extern  const char *YMO_BYTE_NAMES[];
extern  void    ymo_print_ptr           (void *x);
extern  void    ymo_print_sF            (ym_sF x);
extern  void    ymo_print_sF_d3         (ym_sF x);
////////////////////////////////////////////////////////////////////////////////
extern  ym_pL   ymo_print_u32           (ym_u32 x);
extern  ym_pL   ymo_print_s32           (ym_s32 x);
extern  ym_pL   ymo_print_u32_margin    (ym_u32 x,ym_pL margin);
extern  ym_pL   ymo_print_u32_commas    (ym_u32 x,ym_pL margin);
extern  ym_pL   ymo_print_s32_commas    (ym_s32 x,ym_pL margin);
////////////////////////////////////////////////////////////////////////////////
extern  ym_pL   ymo_print_u64           (ym_u64 x);
extern  ym_pL   ymo_print_s64           (ym_s64 x);
extern  ym_pL   ymo_print_u64_margin    (ym_u64 x,ym_pL margin);
extern  ym_pL   ymo_print_s64_margin    (ym_s64 x,ym_pL margin);
extern  ym_pL   ymo_print_u64_commas    (ym_u64 x,ym_pL margin);
extern  ym_pL   ymo_print_s64_commas    (ym_s64 x,ym_pL margin);
////////////////////////////////////////////////////////////////////////////////
extern  void    ymo_print_chs           (const unsigned char *A,ym_pL L);
extern  void    ymo_print_u32s          (const ym_u32 *A,ym_pL L);
extern  void    ymo_print_u32sr         (const ym_u32 *A,ym_pL L);
extern  void    ymo_print_s32s          (const ym_s32 *A,ym_pL L);
extern  void    ymo_print_u64s          (const ym_u64 *A,ym_pL L);
extern  void    ymo_print_u64sr         (const ym_u64 *A,ym_pL L);
extern  void    ymo_print_s64s          (const ym_s64 *A,ym_pL L);
////////////////////////////////////////////////////////////////////////////////
extern  void    ymo_error               (const char *msg);
extern  void    ymo_error_wstr          (const wchar_t *msg);
extern  void    ymo_warning             (const char *msg);
extern  void    ymo_warning_wstr        (const wchar_t *msg);
extern  void    ymo_throw_limit         (ym_uL value,ym_uL limit,const wchar_t *msg);
////////////////////////////////////////////////////////////////////////////////
#define ymo_assert_limit(value,limit,msg){  \
    ym_uL _val = value; \
    ym_uL _lim = limit; \
    if (_val > _lim){   \
        ymo_throw_limit(_val,_lim,msg); \
    }   \
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Other
extern  void    ymo_print_bytes         (ym_uL bytes,int round);
extern  ym_uL   ymo_prompt_uL           (const wchar_t *msg,ym_uL low_limit,ym_uL high_limit);
extern  ym_uL   ymo_prompt_bytes        ();
extern  ym_kL   ymo_prompt_thread_mode  ();
#define ymo_print_uL            ymo_print_u64
//#define ymo_print_uW            ymo_print_u32
//#define ymo_print_uD            ymo_print_u64
//#define ymo_print_sD            ymo_print_s64
//#define ymo_print_uD_commas     ymo_print_u64_commas
//#define ymo_print_sD_commas     ymo_print_s64_commas
//#define ymo_print_uWs           ymo_print_u32s
//#define ymo_print_sWs           ymo_print_s32s
//#define ymo_print_uDs           ymo_print_u64s
//#define ymo_print_sDs           ymo_print_s64s
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
#endif
