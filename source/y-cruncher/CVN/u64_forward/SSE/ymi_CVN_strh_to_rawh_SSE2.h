/* ymi_CVN_strh_to_rawh_Default.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/06/2012
 * Last Modified    : 04/06/2012
 * 
 * 
 *      ymi             =   Micro-Intrinsic
 *      CVN             =   Format Conversion Unit
 *      strh_to_rawh    =   Hexadecimal String -> Hexadecimal Raw
 *      Default         =   Default Instruction Set
 * 
 */

#ifndef _ymi_CVN_strh_to_rawh_SSE_H
#define _ymi_CVN_strh_to_rawh_SSE_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ymi_CVN_strh_to_rawh_u4_SSE2(T,bad){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    const __m128i _LIMIT1 = _mm_set1_epi8('9'); \
    const __m128i _LIMIT2 = _mm_set1_epi8('a'); \
    const __m128i _LIMIT3 = _mm_set1_epi8('f'); \
    const __m128i _HEXDIF = _mm_set1_epi8(39);  \
    \
    register __m128i _a0,_b0,_c0;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    \
    _b0 = _mm_cmplt_epi8(_a0,_LIMIT2);  \
    _c0 = _mm_cmpgt_epi8(_a0,_LIMIT1);  \
    _b0 = _mm_and_si128(_b0,_c0);   \
    bad = _mm_or_si128(bad,_b0);    \
    \
    _b0 = _mm_cmplt_epi8(_a0,_LIMIT0);  \
    bad = _mm_or_si128(bad,_b0);    \
    \
    _b0 = _mm_cmpgt_epi8(_a0,_LIMIT3);  \
    \
    _a0 = _mm_sub_epi8(_a0,_LIMIT0);    \
    _c0 = _mm_and_si128(_c0,_HEXDIF);   \
    \
    bad = _mm_or_si128(bad,_b0);    \
    _a0 = _mm_sub_epi8(_a0,_c0);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
}
#define ymi_CVN_strh_to_rawh_u5_SSE2(T,bad){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    const __m128i _LIMIT1 = _mm_set1_epi8('9'); \
    const __m128i _LIMIT2 = _mm_set1_epi8('a'); \
    const __m128i _LIMIT3 = _mm_set1_epi8('f'); \
    const __m128i _HEXDIF = _mm_set1_epi8(39);  \
    \
    register __m128i _a0,_b0,_c0;   \
    register __m128i _a1,_b1,_c1;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    _a1 = ((const __m128i*)(T))[1]; \
    \
    _b0 = _mm_cmplt_epi8(_a0,_LIMIT2);  \
    _b1 = _mm_cmplt_epi8(_a1,_LIMIT2);  \
    _c0 = _mm_cmpgt_epi8(_a0,_LIMIT1);  \
    _c1 = _mm_cmpgt_epi8(_a1,_LIMIT1);  \
    _b0 = _mm_and_si128(_b0,_c0);   \
    _b1 = _mm_and_si128(_b1,_c1);   \
    bad = _mm_or_si128(bad,_b0);    \
    bad = _mm_or_si128(bad,_b1);    \
    \
    _b0 = _mm_cmplt_epi8(_a0,_LIMIT0);  \
    _b1 = _mm_cmplt_epi8(_a1,_LIMIT0);  \
    bad = _mm_or_si128(bad,_b0);    \
    bad = _mm_or_si128(bad,_b1);    \
    \
    _b0 = _mm_cmpgt_epi8(_a0,_LIMIT3);  \
    _b1 = _mm_cmpgt_epi8(_a1,_LIMIT3);  \
    \
    _a0 = _mm_sub_epi8(_a0,_LIMIT0);    \
    _a1 = _mm_sub_epi8(_a1,_LIMIT0);    \
    _c0 = _mm_and_si128(_c0,_HEXDIF);   \
    _c1 = _mm_and_si128(_c1,_HEXDIF);   \
    \
    bad = _mm_or_si128(bad,_b0);    \
    bad = _mm_or_si128(bad,_b1);    \
    _a0 = _mm_sub_epi8(_a0,_c0);    \
    _a1 = _mm_sub_epi8(_a1,_c1);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
    ((__m128i*)(T))[1] = _a1;   \
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
