/* ymi_CVN_rawd_to_ub_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/22/2011
 * Last Modified    : 01/22/2011
 * 
 * 
 *      ymi         =   Micro-Intrinsic
 *      CVN         =   Format Conversion Unit
 *      rawd_to_ud  =   Decimal Raw -> Binary Integer
 *      SSE41       =   SSE4.1 Instruction Set
 * 
 */

#ifndef _ymi_CVN_rawd_to_ud_SSE41_H
#define _ymi_CVN_rawd_to_ud_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ymi_CVN_rawd_to_u64d19_u1_SSE41(T, A){   \
    const __m128i _MASK0 = _mm_set1_epi16(0xf); \
    const __m128i _MASK1 = _mm_set1_epi32(0xff);    \
    const __m128i _MASK2 = _mm_set_epi32(0, 0xffff, 0, 0xffff);    \
    const __m128i _MASK3 = _mm_set1_epi32(0x00ffffff);  \
    const __m128i _TEN0 = _mm_set1_epi16(10);   \
    const __m128i _TEN1 = _mm_set1_epi32(100);  \
    const __m128i _TEN2 = _mm_set1_epi32(10000);    \
    const __m128i _TEN3 = _mm_set1_epi32(100000000);    \
    const __m128i _FIVE0 = _mm_set1_epi32(78125);   \
    const __m128i _FIVE1 = _mm_set_epi32(0, 390625, 0, 390625);    \
    \
    register __m128i _a0, _b0, _c0;   \
    register __m128i _a1, _b1;   \
    \
    _c0 = _mm_cvtsi32_si128(*(u32_t*)((const char*)(A) +  0));  \
    _c0 = _mm_insert_epi32(_c0, *(u32_t*)((const char*)(A) + 19), 2); \
    _c0 = _mm_and_si128(_c0, _MASK3);    \
    \
    _b0 = _mm_and_si128(_c0, _MASK0);    \
    _c0 = _mm_srli_epi16(_c0, 8);    \
    _b0 = _mm_mullo_epi16(_b0, _TEN0);   \
    _c0 = _mm_add_epi16(_c0, _b0);   \
    \
    _b0 = _mm_and_si128(_c0, _MASK1);    \
    _c0 = _mm_srli_epi32(_c0, 16);   \
    _b0 = _mm_mullo_epi16(_b0, _TEN1);   \
    _c0 = _mm_add_epi16(_c0, _b0);   \
    \
    _c0 = _mm_mul_epu32(_c0, _FIVE0);    \
    _c0 = _mm_mul_epu32(_c0, _FIVE1);    \
    _c0 = _mm_slli_epi64(_c0, 15);   \
    \
    \
    _a0 = _mm_lddqu_si128((__m128i*)((const char*)(A) +  3));   \
    _a1 = _mm_lddqu_si128((__m128i*)((const char*)(A) + 22));   \
    \
    _b0 = _mm_and_si128(_a0, _MASK0);    \
    _b1 = _mm_and_si128(_a1, _MASK0);    \
    _a0 = _mm_srli_epi16(_a0, 8);    \
    _a1 = _mm_srli_epi16(_a1, 8);    \
    _b0 = _mm_mullo_epi16(_b0, _TEN0);   \
    _b1 = _mm_mullo_epi16(_b1, _TEN0);   \
    _a0 = _mm_add_epi16(_a0, _b0);   \
    _a1 = _mm_add_epi16(_a1, _b1);   \
    \
    _b0 = _mm_and_si128(_a0, _MASK1);    \
    _b1 = _mm_and_si128(_a1, _MASK1);    \
    _a0 = _mm_srli_epi32(_a0, 16);   \
    _a1 = _mm_srli_epi32(_a1, 16);   \
    _b0 = _mm_mullo_epi16(_b0, _TEN1);   \
    _b1 = _mm_mullo_epi16(_b1, _TEN1);   \
    _a0 = _mm_add_epi16(_a0, _b0);   \
    _a1 = _mm_add_epi16(_a1, _b1);   \
    \
    _b0 = _mm_and_si128(_a0, _MASK2);    \
    _b1 = _mm_and_si128(_a1, _MASK2);    \
    _a0 = _mm_srli_epi64(_a0, 32);   \
    _a1 = _mm_srli_epi64(_a1, 32);   \
    _b0 = _mm_mullo_epi32(_b0, _TEN2);   \
    _b1 = _mm_mullo_epi32(_b1, _TEN2);   \
    _a0 = _mm_add_epi32(_a0, _b0);   \
    _a1 = _mm_add_epi32(_a1, _b1);   \
    \
    \
    _b0 = _mm_unpacklo_epi64(_a0, _a1);  \
    _a0 = _mm_unpackhi_epi64(_a0, _a1);  \
    _b0 = _mm_mul_epu32(_b0, _TEN3); \
    _a0 = _mm_add_epi64(_a0, _b0);   \
    \
    \
    _a0 = _mm_add_epi64(_a0, _c0);   \
    ((__m128i*)(T))[0] = _a0;   \
}
#define ymi_CVN_rawd_to_u64d19_u2_SSE41(T, A){   \
    const __m128i _MASK0 = _mm_set1_epi16(0xf); \
    const __m128i _MASK1 = _mm_set1_epi32(0xff);    \
    const __m128i _MASK2 = _mm_set_epi32(0, 0xffff, 0, 0xffff);    \
    const __m128i _MASK3 = _mm_set1_epi32(0x00ffffff);  \
    const __m128i _TEN0 = _mm_set1_epi16(10);   \
    const __m128i _TEN1 = _mm_set1_epi32(100);  \
    const __m128i _TEN2 = _mm_set1_epi32(10000);    \
    const __m128i _TEN3 = _mm_set1_epi32(100000000);    \
    const __m128i _FIVE0 = _mm_set1_epi32(78125);   \
    const __m128i _FIVE1 = _mm_set_epi32(0, 390625, 0, 390625);    \
    \
    register __m128i _a0, _b0, _c0;   \
    register __m128i _a1, _b1;   \
    register __m128i _a2, _b2, _c2;   \
    register __m128i _a3, _b3;   \
    \
    _a0 = _mm_lddqu_si128((__m128i*)((const char*)(A) +  3));   \
    \
    _c0 = _mm_cvtsi32_si128(*(u32_t*)((const char*)(A) +  0));  \
    _c0 = _mm_insert_epi32(_c0, *(u32_t*)((const char*)(A) + 19), 2); \
    _c0 = _mm_insert_epi32(_c0, *(u32_t*)((const char*)(A) + 38), 1); \
    _c0 = _mm_insert_epi32(_c0, *(u32_t*)((const char*)(A) + 57), 3); \
    _c0 = _mm_and_si128(_c0, _MASK3);    \
    \
    _a1 = _mm_lddqu_si128((__m128i*)((const char*)(A) + 22));   \
    _c2 = _mm_and_si128(_c0, _MASK0);    \
    _c0 = _mm_srli_epi16(_c0, 8);    \
    _c2 = _mm_mullo_epi16(_c2, _TEN0);   \
    _c0 = _mm_add_epi16(_c0, _c2);   \
    \
    _a2 = _mm_lddqu_si128((__m128i*)((const char*)(A) + 41));   \
    _c2 = _mm_and_si128(_c0, _MASK1);    \
    _c0 = _mm_srli_epi32(_c0, 16);   \
    _c2 = _mm_mullo_epi16(_c2, _TEN1);   \
    _c0 = _mm_add_epi16(_c0, _c2);   \
    \
    _a3 = _mm_lddqu_si128((__m128i*)((const char*)(A) + 60));   \
    _c0 = _mm_mullo_epi32(_c0, _FIVE0);  \
    _c2 = _mm_shuffle_epi32(_c0, 177);   \
    _c0 = _mm_mul_epu32(_c0, _FIVE1);    \
    _c2 = _mm_mul_epu32(_c2, _FIVE1);    \
    _c0 = _mm_slli_epi64(_c0, 15);   \
    _c2 = _mm_slli_epi64(_c2, 15);   \
    \
    \
    _b0 = _mm_and_si128(_a0, _MASK0);    \
    _b1 = _mm_and_si128(_a1, _MASK0);    \
    _b2 = _mm_and_si128(_a2, _MASK0);    \
    _b3 = _mm_and_si128(_a3, _MASK0);    \
    _a0 = _mm_srli_epi16(_a0, 8);    \
    _a1 = _mm_srli_epi16(_a1, 8);    \
    _a2 = _mm_srli_epi16(_a2, 8);    \
    _a3 = _mm_srli_epi16(_a3, 8);    \
    _b0 = _mm_mullo_epi16(_b0, _TEN0);   \
    _b1 = _mm_mullo_epi16(_b1, _TEN0);   \
    _b2 = _mm_mullo_epi16(_b2, _TEN0);   \
    _b3 = _mm_mullo_epi16(_b3, _TEN0);   \
    _a0 = _mm_add_epi16(_a0, _b0);   \
    _a1 = _mm_add_epi16(_a1, _b1);   \
    _a2 = _mm_add_epi16(_a2, _b2);   \
    _a3 = _mm_add_epi16(_a3, _b3);   \
    \
    _b0 = _mm_and_si128(_a0, _MASK1);    \
    _b1 = _mm_and_si128(_a1, _MASK1);    \
    _b2 = _mm_and_si128(_a2, _MASK1);    \
    _b3 = _mm_and_si128(_a3, _MASK1);    \
    _a0 = _mm_srli_epi32(_a0, 16);   \
    _a1 = _mm_srli_epi32(_a1, 16);   \
    _a2 = _mm_srli_epi32(_a2, 16);   \
    _a3 = _mm_srli_epi32(_a3, 16);   \
    _b0 = _mm_mullo_epi16(_b0, _TEN1);   \
    _b1 = _mm_mullo_epi16(_b1, _TEN1);   \
    _b2 = _mm_mullo_epi16(_b2, _TEN1);   \
    _b3 = _mm_mullo_epi16(_b3, _TEN1);   \
    _a0 = _mm_add_epi16(_a0, _b0);   \
    _a1 = _mm_add_epi16(_a1, _b1);   \
    _a2 = _mm_add_epi16(_a2, _b2);   \
    _a3 = _mm_add_epi16(_a3, _b3);   \
    \
    _b0 = _mm_and_si128(_a0, _MASK2);    \
    _b1 = _mm_and_si128(_a1, _MASK2);    \
    _b2 = _mm_and_si128(_a2, _MASK2);    \
    _b3 = _mm_and_si128(_a3, _MASK2);    \
    _a0 = _mm_srli_epi64(_a0, 32);   \
    _a1 = _mm_srli_epi64(_a1, 32);   \
    _a2 = _mm_srli_epi64(_a2, 32);   \
    _a3 = _mm_srli_epi64(_a3, 32);   \
    _b0 = _mm_mullo_epi32(_b0, _TEN2);   \
    _b1 = _mm_mullo_epi32(_b1, _TEN2);   \
    _b2 = _mm_mullo_epi32(_b2, _TEN2);   \
    _b3 = _mm_mullo_epi32(_b3, _TEN2);   \
    _a0 = _mm_add_epi32(_a0, _b0);   \
    _a1 = _mm_add_epi32(_a1, _b1);   \
    _a2 = _mm_add_epi32(_a2, _b2);   \
    _a3 = _mm_add_epi32(_a3, _b3);   \
    \
    \
    _b0 = _mm_unpacklo_epi64(_a0, _a1);  \
    _b2 = _mm_unpacklo_epi64(_a2, _a3);  \
    _a0 = _mm_unpackhi_epi64(_a0, _a1);  \
    _a2 = _mm_unpackhi_epi64(_a2, _a3);  \
    _b0 = _mm_mul_epu32(_b0, _TEN3); \
    _b2 = _mm_mul_epu32(_b2, _TEN3); \
    _a0 = _mm_add_epi64(_a0, _b0);   \
    _a2 = _mm_add_epi64(_a2, _b2);   \
    \
    \
    _a0 = _mm_add_epi64(_a0, _c0);   \
    _a2 = _mm_add_epi64(_a2, _c2);   \
    ((__m128i*)(T))[0] = _a0;   \
    ((__m128i*)(T))[1] = _a2;   \
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
