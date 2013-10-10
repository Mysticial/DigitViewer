/* ymb_CVN_u64_forward.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/07/2012
 * Last Modified    : 04/07/2012
 * 
 * 
 *      ymb         =   Basic Function
 *      CVN         =   Format Conversion Unit
 * 
 */

#pragma once
#ifndef _ymb_CVN_u64_forward_H
#define _ymb_CVN_u64_forward_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "ymb_CVN_headers.h"
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
#if     defined ymc_NPO_SSE41
#include <smmintrin.h>
#include "u64_forward/SSE/ymh_CVN_str_to_raw_SSE2.h"
#include "u64_forward/SSE/ymh_CVN_raw_to_str_SSE2.h"
#include "u64_forward/SSE/ymh_CVN_raw_to_u64_SSE41.h"
#include "u64_forward/SSE/ymh_CVN_u64_to_raw_SSE41.h"
#include "u64_forward/SSE/ymh_CVN_u64_to_str_SSE41.h"
void yma_CVN_u64_forward(){
    ymo_print_label("CVN_u64_forward:\t","SSE4.1",'T');
    ymo_print_label("    ","ymh_CVN_str_to_raw_SSE2.h",'B');
    ymo_print_label("    ","ymh_CVN_raw_to_str_SSE2.h",'B');
    ymo_print_label("    ","ymh_CVN_raw_to_u64_SSE41.h",'T');
    ymo_print_label("    ","ymh_CVN_u64_to_raw_SSE41.h",'T');
    ymo_print_label("    ","ymh_CVN_u64_to_str_SSE41.h",'T');
    ymo_print_astr("\n");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif   defined ymc_NPO_SSE2
#include <emmintrin.h>
#include "u64_forward/SSE/ymh_CVN_str_to_raw_SSE2.h"
#include "u64_forward/SSE/ymh_CVN_raw_to_str_SSE2.h"
#include "u64_forward/SSE/ymh_CVN_raw_to_u64_SSE2.h"
#include "u64_forward/Default/ymh_CVN_u64_to_raw_Default.h"
#include "u64_forward/Default/ymh_CVN_u64_to_str_Default.h"
void yma_CVN_u64_forward(){
    ymo_print_label("CVN_u64_forward:\t","SSE2",'B');
    ymo_print_label("    ","ymh_CVN_str_to_raw_SSE2.h",'B');
    ymo_print_label("    ","ymh_CVN_raw_to_str_SSE2.h",'B');
    ymo_print_label("    ","ymh_CVN_raw_to_u64_SSE2.h",'B');
    ymo_print_label("    ","ymh_CVN_u64_to_raw_Default.h",'w');
    ymo_print_label("    ","ymh_CVN_u64_to_str_Default.h",'w');
    ymo_print_astr("\n");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif (defined _M_X64) || (defined __x86_64)
#include "u64_forward/Default/ymh_CVN_str_to_raw_Default64.h"
#include "u64_forward/Default/ymh_CVN_raw_to_str_Default64.h"
#include "u64_forward/Default/ymh_CVN_raw_to_u64_Default.h"
#include "u64_forward/Default/ymh_CVN_u64_to_raw_Default.h"
#include "u64_forward/Default/ymh_CVN_u64_to_str_Default.h"
void yma_CVN_u64_forward(){
    ymo_print_label("CVN_u64_forward:\t","x64 Default",'B');
    ymo_print_label("    ","ymh_CVN_str_to_raw_Default64.h",'w');
    ymo_print_label("    ","ymh_CVN_raw_to_str_Default64.h",'w');
    ymo_print_label("    ","ymh_CVN_raw_to_u64_Default.h",'w');
    ymo_print_label("    ","ymh_CVN_u64_to_raw_Default.h",'w');
    ymo_print_label("    ","ymh_CVN_u64_to_str_Default.h",'w');
    ymo_print_astr("\n");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#else
#include "u64_forward/Default/ymh_CVN_str_to_raw_Default32.h"
#include "u64_forward/Default/ymh_CVN_raw_to_str_Default32.h"
#include "u64_forward/Default/ymh_CVN_raw_to_u64_Default.h"
#include "u64_forward/Default/ymh_CVN_u64_to_raw_Default.h"
#include "u64_forward/Default/ymh_CVN_u64_to_str_Default.h"
void yma_CVN_u64_forward(){
    ymo_print_label("CVN_u64_forward:\t","x86 Default",'w');
    ymo_print_label("    ","ymh_CVN_str_to_raw_Default32.h",'w');
    ymo_print_label("    ","ymh_CVN_raw_to_str_Default32.h",'w');
    ymo_print_label("    ","ymh_CVN_raw_to_u64_Default.h",'w');
    ymo_print_label("    ","ymh_CVN_u64_to_raw_Default.h",'w');
    ymo_print_label("    ","ymh_CVN_u64_to_str_Default.h",'w');
    ymo_print_astr("\n");
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
#endif
