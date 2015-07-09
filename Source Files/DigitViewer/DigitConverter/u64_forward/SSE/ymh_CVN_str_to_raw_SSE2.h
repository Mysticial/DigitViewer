/* ymi_CVN_str_to_raw_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/06/2012
 * Last Modified    : 04/06/2012
 * 
 * 
 *      ymi             =   Micro-Intrinsic
 *      CVN             =   Format Conversion Unit
 *      str_to_raw      =   String -> Raw
 *      SSE2            =   SSE2 Instruction Set
 * 
 */

#ifndef _ymh_CVN_str_to_raw_SSE2_H
#define _ymh_CVN_str_to_raw_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "../../../x86_SSE2.h"
#include "../../ymb_CVN_headers.h"
#include "../Default/ymi_CVN_strh_to_rawh_Default.h"
#include "../Default/ymi_CVN_strd_to_rawd_Default.h"
#include "ymi_CVN_strh_to_rawh_SSE2.h"
#include "ymi_CVN_strd_to_rawd_SSE2.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int ymb_CVN_strh_to_rawh_f(char* T, upL_t L){
    char* stop = T + L;

    //  Iterate until it is aligned.
    u64_t bad0 = 0;
    while (T < stop && (upL_t)T & 15){
        ymi_CVN_strh_to_rawh_u0_Default(T, bad0);
        T += 1;
    }

    //  Continue at full speed.
    __m128i bad1 = _mm_setzero_si128();
    while (T < stop - 31){
        ymi_CVN_strh_to_rawh_u5_SSE2(T, bad1);
        T += 32;
    }
    while (T < stop - 15){
        ymi_CVN_strh_to_rawh_u4_SSE2(T, bad1);
        T += 16;
    }
    while (T < stop - 7){
        ymi_CVN_strh_to_rawh_u3_Default64(T, bad0);
        T += 8;
    }
    while (T < stop - 3){
        ymi_CVN_strh_to_rawh_u2_Default32(T, bad0);
        T += 4;
    }
    while (T < stop){
        ymi_CVN_strh_to_rawh_u0_Default(T, bad0);
        T += 1;
    }

    u64_t r0 = _mm_cvtsi128_si64(bad1);
    u64_t r1 = _mm_cvtsi128_si64(_mm_unpackhi_epi64(bad1, bad1));
    u64_t bad = bad0 | r0 | r1;

    return bad == 0 ? 0 : 1;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int ymb_CVN_strd_to_rawd_f(char* T, upL_t L){
    char* stop = T + L;

    //  Iterate until it is aligned.
    u64_t bad0 = 0;
    while (T < stop && (upL_t)T & 15){
        ymi_CVN_strd_to_rawd_u0_Default(T, bad0);
        T += 1;
    }

    //  Continue at full speed.
    __m128i bad1 = _mm_setzero_si128();
    while (T < stop - 31){
        ymi_CVN_strd_to_rawd_u5_SSE2(T, bad1);
        T += 32;
    }
    while (T < stop - 15){
        ymi_CVN_strd_to_rawd_u4_SSE2(T, bad1);
        T += 16;
    }
    while (T < stop - 7){
        ymi_CVN_strd_to_rawd_u3_Default64(T, bad0);
        T += 8;
    }
    while (T < stop - 3){
        ymi_CVN_strd_to_rawd_u2_Default32(T, bad0);
        T += 4;
    }
    while (T < stop){
        ymi_CVN_strd_to_rawd_u0_Default(T, bad0);
        T += 1;
    }

    u64_t r0 = _mm_cvtsi128_si64(bad1);
    u64_t r1 = _mm_cvtsi128_si64(_mm_unpackhi_epi64(bad1, bad1));
    u64_t bad = bad0 | r0 | r1;

    return bad == 0 ? 0 : 1;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
