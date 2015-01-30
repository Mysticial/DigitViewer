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
#include "../../Public/ConsoleIO/Label.h"
#include "ymb_CVN_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if     (defined X64_07_Penryn) || (defined YMP_Arch_x64_Penryn)
#include <smmintrin.h>
#include "u64_forward/SSE/ymh_CVN_str_to_raw_SSE2.h"
#include "u64_forward/SSE/ymh_CVN_raw_to_str_SSE2.h"
#include "u64_forward/SSE/ymh_CVN_raw_to_u64_SSE41.h"
#include "u64_forward/SSE/ymh_CVN_u64_to_raw_SSE41.h"
#include "u64_forward/SSE/ymh_CVN_u64_to_str_SSE41.h"
void yma_CVN_u64_forward(){
    Console::println_labelc("CVN_u64_forward","SSE4.1",'T');
    Console::println("    ymh_CVN_str_to_raw_SSE2.h",'B');
    Console::println("    ymh_CVN_raw_to_str_SSE2.h",'B');
    Console::println("    ymh_CVN_raw_to_u64_SSE41.h",'T');
    Console::println("    ymh_CVN_u64_to_raw_SSE41.h",'T');
    Console::println("    ymh_CVN_u64_to_str_SSE41.h",'T');
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif   (defined X86_04_SSE3) || (defined YMP_Arch_x86_SSE2) || (defined X64_04_SSE3) || (defined YMP_Arch_x64_SSE3)
#include "u64_forward/SSE/ymh_CVN_str_to_raw_SSE2.h"
#include "u64_forward/SSE/ymh_CVN_raw_to_str_SSE2.h"
#include "u64_forward/SSE/ymh_CVN_raw_to_u64_SSE2.h"
#include "u64_forward/Default/ymh_CVN_u64_to_raw_Default.h"
#include "u64_forward/Default/ymh_CVN_u64_to_str_Default.h"
void yma_CVN_u64_forward(){
    Console::println_labelc("CVN_u64_forward","SSE2",'B');
    Console::println("    ymh_CVN_str_to_raw_SSE2.h",'B');
    Console::println("    ymh_CVN_raw_to_str_SSE2.h",'B');
    Console::println("    ymh_CVN_raw_to_u64_SSE2.h",'B');
    Console::println("    ymh_CVN_u64_to_raw_Default.h",'w');
    Console::println("    ymh_CVN_u64_to_str_Default.h",'w');
    Console::println();
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
    Console::println_labelc("CVN_u64_forward","x64 Default",'B');
    Console::println("    ymh_CVN_str_to_raw_Default64.h",'w');
    Console::println("    ymh_CVN_raw_to_str_Default64.h",'w');
    Console::println("    ymh_CVN_raw_to_u64_Default.h",'w');
    Console::println("    ymh_CVN_u64_to_raw_Default.h",'w');
    Console::println("    ymh_CVN_u64_to_str_Default.h",'w');
    Console::println();
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
    Console::println_labelc("CVN_u64_forward","x86 Default",'w');
    Console::println("    ymh_CVN_str_to_raw_Default32.h",'w');
    Console::println("    ymh_CVN_raw_to_str_Default32.h",'w');
    Console::println("    ymh_CVN_raw_to_u64_Default.h",'w');
    Console::println("    ymh_CVN_u64_to_raw_Default.h",'w');
    Console::println("    ymh_CVN_u64_to_str_Default.h",'w');
    Console::println();
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
