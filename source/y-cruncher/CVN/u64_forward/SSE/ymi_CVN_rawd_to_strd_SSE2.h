/* ymi_CVN_rawd_to_strd_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/04/2012
 * Last Modified    : 04/06/2012
 * 
 * 
 *      ymi             =   Micro-Intrinsic
 *      CVN             =   Format Conversion Unit
 *      rawd_to_strd    =   Decimal Raw -> Decimal String
 *      SSE2            =   SSE2 Instruction Set
 * 
 */

#ifndef _ymi_CVN_rawd_to_strd_SSE2_H
#define _ymi_CVN_rawd_to_strd_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ymi_CVN_rawd_to_strd_u4_SSE2(T){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    \
    register __m128i _a0;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    \
    _a0 = _mm_add_epi8(_a0,_LIMIT0);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
}
#define ymi_CVN_rawd_to_strd_u5_SSE2(T){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    \
    register __m128i _a0;   \
    register __m128i _a1;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    _a1 = ((const __m128i*)(T))[1]; \
    \
    _a0 = _mm_add_epi8(_a0,_LIMIT0);    \
    _a1 = _mm_add_epi8(_a1,_LIMIT0);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
    ((__m128i*)(T))[1] = _a1;   \
}
#define ymi_CVN_rawd_to_strd_u6_SSE2(T){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    \
    register __m128i _a0;   \
    register __m128i _a1;   \
    register __m128i _a2;   \
    register __m128i _a3;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    _a1 = ((const __m128i*)(T))[1]; \
    _a2 = ((const __m128i*)(T))[2]; \
    _a3 = ((const __m128i*)(T))[3]; \
    \
    _a0 = _mm_add_epi8(_a0,_LIMIT0);    \
    _a1 = _mm_add_epi8(_a1,_LIMIT0);    \
    _a2 = _mm_add_epi8(_a2,_LIMIT0);    \
    _a3 = _mm_add_epi8(_a3,_LIMIT0);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
    ((__m128i*)(T))[1] = _a1;   \
    ((__m128i*)(T))[2] = _a2;   \
    ((__m128i*)(T))[3] = _a3;   \
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
