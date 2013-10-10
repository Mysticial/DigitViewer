/* ymi_CVN_rawh_to_strh_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/06/2012
 * Last Modified    : 04/06/2012
 * 
 * 
 *      ymi             =   Micro-Intrinsic
 *      CVN             =   Format Conversion Unit
 *      rawh_to_strh    =   Hexadecimal Raw -> Hexadecimal String
 *      SSE2            =   SSE2 Instruction Set
 * 
 */

#ifndef _ymi_CVN_rawh_to_strh_SSE2_H
#define _ymi_CVN_rawh_to_strh_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ymi_CVN_rawh_to_strh_u4_SSE2(T){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    const __m128i _LIMIT1 = _mm_set1_epi8(9);   \
    const __m128i _HEXDIF = _mm_set1_epi8(39);  \
    \
    register __m128i _a0,_b0;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    \
    _b0 = _mm_cmpgt_epi8(_a0,_LIMIT1);  \
    \
    _a0 = _mm_add_epi8(_a0,_LIMIT0);    \
    \
    _b0 = _mm_and_si128(_b0,_HEXDIF);   \
    \
    _a0 = _mm_add_epi8(_a0,_b0);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
}
#define ymi_CVN_rawh_to_strh_u5_SSE2(T){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    const __m128i _LIMIT1 = _mm_set1_epi8(9);   \
    const __m128i _HEXDIF = _mm_set1_epi8(39);  \
    \
    register __m128i _a0,_b0;   \
    register __m128i _a1,_b1;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    _a1 = ((const __m128i*)(T))[1]; \
    \
    _b0 = _mm_cmpgt_epi8(_a0,_LIMIT1);  \
    _b1 = _mm_cmpgt_epi8(_a1,_LIMIT1);  \
    \
    _a0 = _mm_add_epi8(_a0,_LIMIT0);    \
    _a1 = _mm_add_epi8(_a1,_LIMIT0);    \
    \
    _b0 = _mm_and_si128(_b0,_HEXDIF);   \
    _b1 = _mm_and_si128(_b1,_HEXDIF);   \
    \
    _a0 = _mm_add_epi8(_a0,_b0);    \
    _a1 = _mm_add_epi8(_a1,_b1);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
    ((__m128i*)(T))[1] = _a1;   \
}
#define ymi_CVN_rawh_to_strh_u6_SSE2(T){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    const __m128i _LIMIT1 = _mm_set1_epi8(9);   \
    const __m128i _HEXDIF = _mm_set1_epi8(39);  \
    \
    register __m128i _a0,_b0;   \
    register __m128i _a1,_b1;   \
    register __m128i _a2,_b2;   \
    register __m128i _a3,_b3;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    _a1 = ((const __m128i*)(T))[1]; \
    _a2 = ((const __m128i*)(T))[2]; \
    _a3 = ((const __m128i*)(T))[3]; \
    \
    _b0 = _mm_cmpgt_epi8(_a0,_LIMIT1);  \
    _b1 = _mm_cmpgt_epi8(_a1,_LIMIT1);  \
    _b2 = _mm_cmpgt_epi8(_a2,_LIMIT1);  \
    _b3 = _mm_cmpgt_epi8(_a3,_LIMIT1);  \
    \
    _a0 = _mm_add_epi8(_a0,_LIMIT0);    \
    _a1 = _mm_add_epi8(_a1,_LIMIT0);    \
    _a2 = _mm_add_epi8(_a2,_LIMIT0);    \
    _a3 = _mm_add_epi8(_a3,_LIMIT0);    \
    \
    _b0 = _mm_and_si128(_b0,_HEXDIF);   \
    _b1 = _mm_and_si128(_b1,_HEXDIF);   \
    _b2 = _mm_and_si128(_b2,_HEXDIF);   \
    _b3 = _mm_and_si128(_b3,_HEXDIF);   \
    \
    _a0 = _mm_add_epi8(_a0,_b0);    \
    _a1 = _mm_add_epi8(_a1,_b1);    \
    _a2 = _mm_add_epi8(_a2,_b2);    \
    _a3 = _mm_add_epi8(_a3,_b3);    \
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
