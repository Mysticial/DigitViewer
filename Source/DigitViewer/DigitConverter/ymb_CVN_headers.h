/* ymb_CVN_headers.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 11/09/2011
 * Last Modified    : 04/07/2012
 * 
 * 
 *      ymb         =   Basic Function
 *      CVN         =   Format Conversion Unit
 * 
 */

#pragma once
#ifndef _ymb_CVN_headers_H
#define _ymb_CVN_headers_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Types.h"
using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  u64_forward (use by Digit Viewer)
void    yma_CVN_u64_forward         ();
void    ymb_CVN_rawh_to_strh_f      (char* T, upL_t L);
void    ymb_CVN_rawd_to_strd_f      (char* T, upL_t L);
int     ymb_CVN_strh_to_rawh_f      (char* T, upL_t L);
int     ymb_CVN_strd_to_rawd_f      (char* T, upL_t L);
void    ymb_CVN_rawh_to_u64b_f      (u64_t* T, const char* A, upL_t TL);
void    ymb_CVN_rawd_to_u64d_f      (u64_t* T, const char* A, upL_t TL);
void    ymb_CVN_u64b_to_rawh_f      (char* T, const u64_t* A, upL_t AL);
void    ymb_CVN_u64d_to_rawd_f      (char* T, const u64_t* A, upL_t AL);
void    ymb_CVN_u64b_to_strh_f      (char* T, const u64_t* A, upL_t AL);
void    ymb_CVN_u64d_to_strd_f      (char* T, const u64_t* A, upL_t AL);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  cdi_reverse (use by y-cruncher)
namespace DigitViewer{
void    yma_CVN_cdi_reverse         ();
void    ymb_CVN_ci_to_rawh_r        (char* T, const u32_t* A, upL_t AL);
void    ymb_CVN_ci_to_rawh_r        (char* T, const u64_t* A, upL_t AL);
void    ymb_CVN_ci_to_rawd_r        (char* T, const u32_t* A, upL_t AL);
void    ymb_CVN_ci_to_rawd_r        (char* T, const u64_t* A, upL_t AL);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif