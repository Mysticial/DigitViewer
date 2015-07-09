/* ymi_CVN_rawh_to_ub_SSE2.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/22/2011
 * Last Modified    : 04/06/2012
 * 
 * 
 *      ymi         =   Micro-Intrinsic
 *      CVN         =   Format Conversion Unit
 *      rawh_to_ub  =   Hexadecimal Raw -> Binary Integer
 *      SSE2        =   SSE2 Instruction Set
 * 
 */

#ifndef _ymi_CVN_rawh_to_ub_SSE2_H
#define _ymi_CVN_rawh_to_ub_SSE2_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ymi_CVN_rawh_to_u64b_u0_SSE2(T, A){  \
    const __m128i _MASK0 = _mm_set1_epi16(0xf); \
    const __m128i _MASK1 = _mm_set1_epi32(0xff);    \
    \
    register __m128i _a0;   \
    register __m128i _b0;   \
    \
    _a0 = _mm_loadu_si128((const __m128i*)(A) + 0); \
    \
    _b0 = _mm_and_si128(_a0, _MASK0);    \
    _a0 = _mm_srli_epi16(_a0, 8);    \
    _b0 = _mm_slli_epi16(_b0, 4);    \
    _a0 = _mm_or_si128(_a0, _b0);    \
    \
    _b0 = _mm_and_si128(_a0, _MASK1);    \
    _a0 = _mm_srli_epi32(_a0, 16);   \
    _b0 = _mm_slli_epi16(_b0, 8);    \
    _a0 = _mm_or_si128(_a0, _b0);    \
    \
    _a0 = _mm_shufflelo_epi16(_a0, 114); \
    _a0 = _mm_shufflehi_epi16(_a0, 114); \
    _a0 = _mm_shuffle_epi32(_a0, 114);   \
    \
    _mm_storel_epi64((__m128i*)(T) + 0, _a0);    \
}
#define ymi_CVN_rawh_to_u64b_u1_SSE2(T, A){  \
    const __m128i _MASK0 = _mm_set1_epi16(0xf); \
    const __m128i _MASK1 = _mm_set1_epi32(0xff);    \
    \
    register __m128i _a0, _a1;   \
    register __m128i _b0, _b1;   \
    \
    _a0 = _mm_loadu_si128((const __m128i*)(A) + 0); \
    _a1 = _mm_loadu_si128((const __m128i*)(A) + 1); \
    \
    _b0 = _mm_and_si128(_a0, _MASK0);    \
    _b1 = _mm_and_si128(_a1, _MASK0);    \
    _a0 = _mm_srli_epi16(_a0, 8);    \
    _a1 = _mm_srli_epi16(_a1, 8);    \
    \
    _b0 = _mm_slli_epi16(_b0, 4);    \
    _b1 = _mm_slli_epi16(_b1, 4);    \
    \
    _a0 = _mm_or_si128(_a0, _b0);    \
    _a1 = _mm_or_si128(_a1, _b1);    \
    \
    _b0 = _mm_and_si128(_a0, _MASK1);    \
    _b1 = _mm_and_si128(_a1, _MASK1);    \
    _a0 = _mm_srli_epi32(_a0, 16);   \
    _a1 = _mm_srli_epi32(_a1, 16);   \
    \
    _b0 = _mm_slli_epi16(_b0, 8);    \
    _b1 = _mm_slli_epi16(_b1, 8);    \
    \
    _a0 = _mm_or_si128(_a0, _b0);    \
    _a1 = _mm_or_si128(_a1, _b1);    \
    \
    _a0 = _mm_shufflelo_epi16(_a0, 114); \
    _a1 = _mm_shufflelo_epi16(_a1, 114); \
    \
    _a0 = _mm_shufflehi_epi16(_a0, 114); \
    _a1 = _mm_shufflehi_epi16(_a1, 114); \
    \
    _a0 = _mm_shuffle_epi32(_a0, 114);   \
    _a1 = _mm_shuffle_epi32(_a1, 39);    \
    \
    _a0 = _mm_or_si128(_a0, _a1);    \
    ((__m128i*)(T))[0] = _a0;   \
}
#define ymi_CVN_rawh_to_u64b_u2_SSE2(T, A){  \
    const __m128i _MASK0 = _mm_set1_epi16(0xf); \
    const __m128i _MASK1 = _mm_set1_epi32(0xff);    \
    \
    register __m128i _a0, _a1, _a2, _a3;   \
    register __m128i _b0, _b1, _b2, _b3;   \
    \
    _a0 = _mm_loadu_si128((const __m128i*)(A) + 0); \
    _a1 = _mm_loadu_si128((const __m128i*)(A) + 1); \
    _a2 = _mm_loadu_si128((const __m128i*)(A) + 2); \
    _a3 = _mm_loadu_si128((const __m128i*)(A) + 3); \
    \
    _b0 = _mm_and_si128(_a0, _MASK0);    \
    _b1 = _mm_and_si128(_a1, _MASK0);    \
    _b2 = _mm_and_si128(_a2, _MASK0);    \
    _b3 = _mm_and_si128(_a3, _MASK0);    \
    _a0 = _mm_srli_epi16(_a0, 8);    \
    _a1 = _mm_srli_epi16(_a1, 8);    \
    _a2 = _mm_srli_epi16(_a2, 8);    \
    _a3 = _mm_srli_epi16(_a3, 8);    \
    \
    _b0 = _mm_slli_epi16(_b0, 4);    \
    _b1 = _mm_slli_epi16(_b1, 4);    \
    _b2 = _mm_slli_epi16(_b2, 4);    \
    _b3 = _mm_slli_epi16(_b3, 4);    \
    \
    _a0 = _mm_or_si128(_a0, _b0);    \
    _a1 = _mm_or_si128(_a1, _b1);    \
    _a2 = _mm_or_si128(_a2, _b2);    \
    _a3 = _mm_or_si128(_a3, _b3);    \
    \
    _b0 = _mm_and_si128(_a0, _MASK1);    \
    _b1 = _mm_and_si128(_a1, _MASK1);    \
    _b2 = _mm_and_si128(_a2, _MASK1);    \
    _b3 = _mm_and_si128(_a3, _MASK1);    \
    _a0 = _mm_srli_epi32(_a0, 16);   \
    _a1 = _mm_srli_epi32(_a1, 16);   \
    _a2 = _mm_srli_epi32(_a2, 16);   \
    _a3 = _mm_srli_epi32(_a3, 16);   \
    \
    _b0 = _mm_slli_epi16(_b0, 8);    \
    _b1 = _mm_slli_epi16(_b1, 8);    \
    _b2 = _mm_slli_epi16(_b2, 8);    \
    _b3 = _mm_slli_epi16(_b3, 8);    \
    \
    _a0 = _mm_or_si128(_a0, _b0);    \
    _a1 = _mm_or_si128(_a1, _b1);    \
    _a2 = _mm_or_si128(_a2, _b2);    \
    _a3 = _mm_or_si128(_a3, _b3);    \
    \
    _a0 = _mm_shufflelo_epi16(_a0, 114); \
    _a1 = _mm_shufflelo_epi16(_a1, 114); \
    _a2 = _mm_shufflelo_epi16(_a2, 114); \
    _a3 = _mm_shufflelo_epi16(_a3, 114); \
    \
    _a0 = _mm_shufflehi_epi16(_a0, 114); \
    _a1 = _mm_shufflehi_epi16(_a1, 114); \
    _a2 = _mm_shufflehi_epi16(_a2, 114); \
    _a3 = _mm_shufflehi_epi16(_a3, 114); \
    \
    _a0 = _mm_shuffle_epi32(_a0, 114);   \
    _a1 = _mm_shuffle_epi32(_a1, 39);    \
    _a2 = _mm_shuffle_epi32(_a2, 114);   \
    _a3 = _mm_shuffle_epi32(_a3, 39);    \
    \
    _a0 = _mm_or_si128(_a0, _a1);    \
    _a2 = _mm_or_si128(_a2, _a3);    \
    ((__m128i*)(T))[0] = _a0;   \
    ((__m128i*)(T))[1] = _a2;   \
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
