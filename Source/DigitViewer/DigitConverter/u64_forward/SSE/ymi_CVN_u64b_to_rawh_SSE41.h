/* ymi_CVN_ub_to_rawh_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 04/07/2012
 * 
 * 
 *      ymi         =   Micro-Intrinsic
 *      CVN         =   Format Conversion Unit
 *      ub_to_rawh  =   Binary Integer -> Hexadecimal Raw
 *      SSE41       =   SSE4.1 Instruction Set
 * 
 */

#ifndef _ymi_CVN_ub_to_rawh_SSE41_H
#define _ymi_CVN_ub_to_rawh_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ymi_CVN_u64b_to_rawh_u1_SSE41(T, A){ \
    register const __m128i _MASK = _mm_set1_epi16(0xf0);    \
    \
    register __m128i _a0, _a1;   \
    register __m128i _b0, _b1;   \
    \
    _a0 = ((const __m128i*)(A))[0]; \
    _a1 = _mm_shuffle_epi32(_a0, 27);    \
    \
    _a0 = _mm_cvtepu8_epi16(_a0);   \
    _a1 = _mm_cvtepu8_epi16(_a1);   \
    \
    _b0 = _mm_and_si128(_a0, _MASK); \
    _b1 = _mm_and_si128(_a1, _MASK); \
    \
    _a0 = _mm_xor_si128(_a0, _b0);   \
    _a1 = _mm_xor_si128(_a1, _b1);   \
    \
    _b0 = _mm_srli_epi16(_b0, 4);    \
    _b1 = _mm_srli_epi16(_b1, 4);    \
    _a0 = _mm_slli_epi16(_a0, 8);    \
    _a1 = _mm_slli_epi16(_a1, 8);    \
    \
    _a0 = _mm_or_si128(_a0, _b0);    \
    _a1 = _mm_or_si128(_a1, _b1);    \
    \
    _a0 = _mm_shufflelo_epi16(_a0, 27);  \
    _a1 = _mm_shufflelo_epi16(_a1, 27);  \
    \
    _a0 = _mm_shufflehi_epi16(_a0, 27);  \
    _a1 = _mm_shufflehi_epi16(_a1, 27);  \
    \
    _a0 = _mm_shuffle_epi32(_a0, 78);    \
    \
    _mm_storeu_si128((__m128i*)(T) + 0, _a0);    \
    _mm_storeu_si128((__m128i*)(T) + 1, _a1);    \
}
#define ymi_CVN_u64b_to_rawh_u2_SSE41(T, A){ \
    register const __m128i _MASK = _mm_set1_epi16(0xf0);    \
    \
    register __m128i _a0, _a1, _a2, _a3;   \
    register __m128i _b0, _b1, _b2, _b3;   \
    \
    _a0 = ((const __m128i*)(A))[0]; \
    _a2 = ((const __m128i*)(A))[1]; \
    _a1 = _mm_shuffle_epi32(_a0, 27);    \
    _a3 = _mm_shuffle_epi32(_a2, 27);    \
    \
    _a0 = _mm_cvtepu8_epi16(_a0);   \
    _a1 = _mm_cvtepu8_epi16(_a1);   \
    _a2 = _mm_cvtepu8_epi16(_a2);   \
    _a3 = _mm_cvtepu8_epi16(_a3);   \
    \
    _b0 = _mm_and_si128(_a0, _MASK); \
    _b1 = _mm_and_si128(_a1, _MASK); \
    _b2 = _mm_and_si128(_a2, _MASK); \
    _b3 = _mm_and_si128(_a3, _MASK); \
    \
    _a0 = _mm_xor_si128(_a0, _b0);   \
    _a1 = _mm_xor_si128(_a1, _b1);   \
    _a2 = _mm_xor_si128(_a2, _b2);   \
    _a3 = _mm_xor_si128(_a3, _b3);   \
    \
    _b0 = _mm_srli_epi16(_b0, 4);    \
    _b1 = _mm_srli_epi16(_b1, 4);    \
    _b2 = _mm_srli_epi16(_b2, 4);    \
    _b3 = _mm_srli_epi16(_b3, 4);    \
    _a0 = _mm_slli_epi16(_a0, 8);    \
    _a1 = _mm_slli_epi16(_a1, 8);    \
    _a2 = _mm_slli_epi16(_a2, 8);    \
    _a3 = _mm_slli_epi16(_a3, 8);    \
    \
    _a0 = _mm_or_si128(_a0, _b0);    \
    _a1 = _mm_or_si128(_a1, _b1);    \
    _a2 = _mm_or_si128(_a2, _b2);    \
    _a3 = _mm_or_si128(_a3, _b3);    \
    \
    _a0 = _mm_shufflelo_epi16(_a0, 27);  \
    _a1 = _mm_shufflelo_epi16(_a1, 27);  \
    _a2 = _mm_shufflelo_epi16(_a2, 27);  \
    _a3 = _mm_shufflelo_epi16(_a3, 27);  \
    \
    _a0 = _mm_shufflehi_epi16(_a0, 27);  \
    _a1 = _mm_shufflehi_epi16(_a1, 27);  \
    _a2 = _mm_shufflehi_epi16(_a2, 27);  \
    _a3 = _mm_shufflehi_epi16(_a3, 27);  \
    \
    _a0 = _mm_shuffle_epi32(_a0, 78);    \
    _a2 = _mm_shuffle_epi32(_a2, 78);    \
    \
    _mm_storeu_si128((__m128i*)(T) + 0, _a0);    \
    _mm_storeu_si128((__m128i*)(T) + 1, _a1);    \
    _mm_storeu_si128((__m128i*)(T) + 2, _a2);    \
    _mm_storeu_si128((__m128i*)(T) + 3, _a3);    \
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
