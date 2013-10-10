/* ymi_CVN_u64_to_raw_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/22/2011
 * Last Modified    : 04/07/2012
 * 
 * 
 *      ymi         =   Micro-Intrinsic
 *      CVN         =   Format Conversion Unit
 *      u64_to_raw  =   Binary Integer -> Raw
 *      SSE41       =   SSE4.1 Instruction Set
 * 
 */

#ifndef _ymh_CVN_u64_to_raw_SSE41_H
#define _ymh_CVN_u64_to_raw_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "../../ymb_CVN_headers.h"
#include "../Default/ymi_CVN_u64b_to_rawh_Default.h"
#include "../Default/ymi_CVN_u64d_to_rawd_Default.h"
#include "ymi_CVN_u64b_to_rawh_SSE41.h"
#include "ymi_CVN_u64d_to_rawd_SSE41.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymb_CVN_u64b_to_rawh_f(char *T,const ym_u64 *A,ym_pL AL){
    const ym_u64 *stopA = A + AL;

    //  Iterate until A is aligned.
    if (A < stopA && ((ym_pL)A & 15)){
        ymi_CVN_u64b_to_rawh_u0_Default(T,A);
        A += 1;
        T += 16;
    }

    //  Proceed at full speed.
    while (A < stopA - 3){
        ymi_CVN_u64b_to_rawh_u2_SSE41(T,A);
        A += 4;
        T += 64;
    }
    while (A < stopA - 1){
        ymi_CVN_u64b_to_rawh_u1_SSE41(T,A);
        A += 2;
        T += 32;
    }
    if (A < stopA){
        ymi_CVN_u64b_to_rawh_u0_Default(T,A);
        A += 1;
        T += 16;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymb_CVN_u64d_to_rawd_f(char *T,const ym_u64 *A,ym_pL AL){
    const ym_u64 *stopA = A + AL;

    //  Iterate until A is aligned.
    while  (A < stopA && ((ym_pL)A & 15)){
        ymi_CVN_u64d19_to_rawd_u0_Default(T,A);
        A += 1;
        T += 19;
    }

    //  Proceed at full speed.
    while (A < stopA - 1){
        ymi_CVN_u64d19_to_rawd_u1_SSE41(T,A);
        A += 2;
        T += 38;
    }
    if (A < stopA){
        ymi_CVN_u64d19_to_rawd_u0_Default(T,A);
        A += 1;
        T += 19;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
