/* ymi_CVN_raw_to_str_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/06/2012
 * Last Modified    : 04/06/2012
 * 
 * 
 *      ymi             =   Micro-Intrinsic
 *      CVN             =   Format Conversion Unit
 *      raw_to_str      =   Raw -> String
 *      SSE2            =   SSE2 Instruction Set
 * 
 */

#ifndef _ymh_CVN_raw_to_str_SSE2_H
#define _ymh_CVN_raw_to_str_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "DigitViewer/DigitConverter/ymb_CVN_headers.h"
#include "DigitViewer/DigitConverter/u64_forward/Default/ymi_CVN_rawh_to_strh_Default.h"
#include "DigitViewer/DigitConverter/u64_forward/Default/ymi_CVN_rawd_to_strd_Default.h"
#include "ymi_CVN_rawh_to_strh_SSE2.h"
#include "ymi_CVN_rawd_to_strd_SSE2.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymb_CVN_rawh_to_strh_f(char* T, upL_t L){
    char* stop = T + L;
    
    while (T < stop - 63){
        ymi_CVN_rawh_to_strh_u6_SSE2(T);
        T += 64;
    }
    while (T < stop - 31){
        ymi_CVN_rawh_to_strh_u5_SSE2(T);
        T += 32;
    }
    while (T < stop - 15){
        ymi_CVN_rawh_to_strh_u4_SSE2(T);
        T += 16;
    }
    while (T < stop - 7){
        ymi_CVN_rawh_to_strh_u3_Default64(T);
        T += 8;
    }
    while (T < stop - 3){
        ymi_CVN_rawh_to_strh_u2_Default32(T);
        T += 4;
    }
    while (T < stop){
        ymi_CVN_rawh_to_strh_u0_Default(T);
        T += 1;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymb_CVN_rawd_to_strd_f(char* T, upL_t L){
    char* stop = T + L;
    
    while (T < stop - 63){
        ymi_CVN_rawd_to_strd_u6_SSE2(T);
        T += 64;
    }
    while (T < stop - 31){
        ymi_CVN_rawd_to_strd_u5_SSE2(T);
        T += 32;
    }
    while (T < stop - 15){
        ymi_CVN_rawd_to_strd_u4_SSE2(T);
        T += 16;
    }
    while (T < stop - 7){
        ymi_CVN_rawd_to_strd_u3_Default64(T);
        T += 8;
    }
    while (T < stop - 3){
        ymi_CVN_rawd_to_strd_u2_Default32(T);
        T += 4;
    }
    while (T < stop){
        ymi_CVN_rawd_to_strd_u0_Default(T);
        T += 1;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
