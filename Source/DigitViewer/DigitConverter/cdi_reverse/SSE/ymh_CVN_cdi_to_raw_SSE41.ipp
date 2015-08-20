/* ymh_CVN_cdi_to_raw_SSE41.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/20/2011
 * Last Modified    : 05/31/2012
 * 
 * 
 *      ymh         =   Helper Function
 *      CVN         =   Format Conversion Unit
 *      cdi_to_raw  =   Compressed Integer -> Raw
 *      SSE41       =   SSE4.1 Instruction Set
 * 
 * 
 *      This file implements a word-sized radix conversion from binary to decimal.
 *  The algorithm used here is a linearized scaled remainder tree.
 * 
 *      The factor "10.00000002" is a multiplicative purturbance to correct for
 *  roundoff error. There is no paper proof that this value works. Instead, it
 *  was verified to work by brute-forcing all possible inputs.
 *  There are only 10 billion combinations: (0 - 9, 999, 999, 999)
 * 
 *  The range of values that work is relatively small - less than an order of
 *  magnitude. (10.000000009 - 10.000000040)
 * 
 * 
 *      The factor "0.00000000000001" is an additive purturbance to correct for
 *  roundoff error. This is verified by brute-forcing all possibilities.
 *  Values in the range of 10^-11 - 10^-15 have been verified to work. The
 *  current implementation uses 10^-13.
 * 
 * 
 * 
 *  AUTHOR's NOTE (Added: 3/29/2012)
 * 
 *      To the poor souls who may be trying to read this file...
 * 
 *  Let's just say that I got a little carried away when I wrote this. This
 *  code isn't even that performance critical, but it was the first time I had
 *  attempted to vectorize something that is (intuitively) not vectorizable.
 * 
 *  So it was more of a learning experience... :-P
 * 
 *  The performance gain of this madness over the straight-foward division code
 *  is only about 1.5 - 3x depending on the architecture.
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "../../../../PublicLibs/Types.h"
#include "../../../../PublicLibs/ConsoleIO/Label.h"
#include "../Default/ymi_CVN_cdi_to_rawh_Default.h"
#include "../Default/ymi_CVN_cdi_to_rawd_Default.h"
#include "ymi_CVN_cdi_to_rawh_SSE41.h"
#include "ymi_CVN_cdi_to_rawd_SSE41.h"
namespace DigitViewer{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void yma_CVN_cdi_reverse(){
    Console::println_labelc("CVN_cdi_reverse", "SSE4.1", 'T');
    Console::println("    ymh_CVN_cdi_to_raw_SSE41.ipp", 'T');
    Console::println();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymb_CVN_ci_to_rawh_r(char* T, const u32_t* A, upL_t AL){
    T += AL * 8;
    const u32_t* stopA = A + AL;

    const upL_t MASK = sizeof(__m128i) - 1;

    //  Align it
    while (A < stopA && ((upL_t)A & MASK)){
        T -= 8;
        ymi_CVN_c32i_to_rawh_u0_Default(T, A);
        A += 1;
    }

    stopA -= 7;
    while (A < stopA){
        T -= 64;
        ymi_CVN_c32i_to_rawh_u3_SSE41(T, A);
        A += 8;
    }
    if (A < stopA + 4){
        T -= 32;
        ymi_CVN_c32i_to_rawh_u2_SSE41(T, A);
        A += 4;
    }
    if (A < stopA + 6){
        T -= 16;
        ymi_CVN_c32i_to_rawh_u1_SSE41(T, A);
        A += 2;
    }
    if (A < stopA + 7){
        T -= 8;
        ymi_CVN_c32i_to_rawh_u0_Default(T, A);
        A += 1;
    }
}
void ymb_CVN_ci_to_rawh_r(char* T, const u64_t* A, upL_t AL){
    T += AL * 16;
    const u64_t* stopA = A + AL;

    const upL_t MASK = sizeof(__m128i) - 1;

    //  Align it
    while (A < stopA && ((upL_t)A & MASK)){
        T -= 16;
        ymi_CVN_c32i_to_rawh_u1_SSE41(T, A);
        A += 1;
    }

    stopA -= 3;
    while (A < stopA){
        T -= 64;
        ymi_CVN_c32i_to_rawh_u3_SSE41(T, A);
        A += 4;
    }
    if (A < stopA + 2){
        T -= 32;
        ymi_CVN_c32i_to_rawh_u2_SSE41(T, A);
        A += 2;
    }
    if (A < stopA + 3){
        T -= 16;
        ymi_CVN_c32i_to_rawh_u1_SSE41(T, A);
        A += 1;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ymb_CVN_ci_to_rawd_r(char* T, const u32_t* A, upL_t AL){
    T += AL * 9;
    const u32_t* stopA = A + AL;

    const upL_t MASK = sizeof(__m128i) - 1;

    //  Align it
    while (A < stopA && ((upL_t)A & MASK)){
        T -= 9;
        ymi_CVN_c32i_to_rawd_u0_Default(T, A);
        A += 1;
    }

    stopA -= 3;
    while (A < stopA){
        T -= 36;
        ymi_CVN_c32i_to_rawd_u2_SSE41(T, A);
        A += 4;
    }
    if (A < stopA + 2){
        T -= 18;
        ymi_CVN_c32i_to_rawd_u1_Default(T, A);
        A += 2;
    }
    if (A < stopA + 3){
        T -= 9;
        ymi_CVN_c32i_to_rawd_u0_Default(T, A);
        A += 1;
    }
}
void ymb_CVN_ci_to_rawd_r(char* T, const u64_t* A, upL_t AL){
    T += AL * 19;
    const u64_t* stopA = A + AL;

    const upL_t MASK = sizeof(__m128i) - 1;

    //  Align it
    while (A < stopA && ((upL_t)A & MASK)){
        T -= 19;
        ymi_CVN_c64i_to_rawd_u0_Default(T, A);
        A += 1;
    }

    stopA -= 1;
    while (A < stopA){
        T -= 38;
        ymi_CVN_c64i_to_rawd_u1_SSE41(T, A);
        A += 2;
    }
    if (A < stopA + 1){
        T -= 19;
        ymi_CVN_c64i_to_rawd_u0_Default(T, A);
        A += 1;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
