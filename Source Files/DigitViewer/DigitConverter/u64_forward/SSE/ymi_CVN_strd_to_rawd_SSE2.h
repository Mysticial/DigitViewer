/* ymi_CVN_strd_to_rawd_Default.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/06/2012
 * Last Modified    : 04/06/2012
 * 
 * 
 *      ymi             =   Micro-Intrinsic
 *      CVN             =   Format Conversion Unit
 *      strd_to_rawd    =   Decimal String -> Decimal Raw
 *      Default         =   Default Instruction Set
 * 
 */

#ifndef _ymi_CVN_strd_to_rawd_SSE_H
#define _ymi_CVN_strd_to_rawd_SSE_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ymi_CVN_strd_to_rawd_u4_SSE2(T, bad){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    const __m128i _LIMIT1 = _mm_set1_epi8('9'); \
    \
    register __m128i _a0, _b0;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    \
    _b0 = _mm_cmplt_epi8(_a0, _LIMIT0);  \
    bad = _mm_or_si128(bad, _b0);    \
    \
    _b0 = _mm_cmpgt_epi8(_a0, _LIMIT1);  \
    bad = _mm_or_si128(bad, _b0);    \
    \
    _a0 = _mm_sub_epi8(_a0, _LIMIT0);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
}
#define ymi_CVN_strd_to_rawd_u5_SSE2(T, bad){    \
    const __m128i _LIMIT0 = _mm_set1_epi8('0'); \
    const __m128i _LIMIT1 = _mm_set1_epi8('9'); \
    \
    register __m128i _a0, _b0;   \
    register __m128i _a1, _b1;   \
    \
    _a0 = ((const __m128i*)(T))[0]; \
    _a1 = ((const __m128i*)(T))[1]; \
    \
    _b0 = _mm_cmplt_epi8(_a0, _LIMIT0);  \
    _b1 = _mm_cmplt_epi8(_a1, _LIMIT0);  \
    bad = _mm_or_si128(bad, _b0);    \
    bad = _mm_or_si128(bad, _b1);    \
    \
    _b0 = _mm_cmpgt_epi8(_a0, _LIMIT1);  \
    _b1 = _mm_cmpgt_epi8(_a1, _LIMIT1);  \
    bad = _mm_or_si128(bad, _b0);    \
    bad = _mm_or_si128(bad, _b1);    \
    \
    _a0 = _mm_sub_epi8(_a0, _LIMIT0);    \
    _a1 = _mm_sub_epi8(_a1, _LIMIT0);    \
    \
    ((__m128i*)(T))[0] = _a0;   \
    ((__m128i*)(T))[1] = _a1;   \
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
