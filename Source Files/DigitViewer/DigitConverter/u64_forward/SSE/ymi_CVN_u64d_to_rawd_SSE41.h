/* ymi_CVN_ud_to_rawd_SSE41.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/19/2011
 * Last Modified    : 04/07/2012
 * 
 * 
 *      ymi         =   Micro-Intrinsic
 *      CVN         =   Format Conversion Unit
 *      ud_to_rawd  =   Binary Integer -> Decimal Raw
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

#ifndef _ymi_CVN_ud_to_rawd_SSE41_H
#define _ymi_CVN_ud_to_rawd_SSE41_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ymi_CVN_u64d19_to_rawd_u1_SSE41(T, A){   \
    const __m128d _SCALE0 = _mm_set1_pd(0.000004096);   \
    /* const __m128d _SCALE1 = _mm_set1_pd(4096.); */   \
    const __m128d _SCALE2 = _mm_set1_pd(0.000000001);   \
    const __m128d _SCALE3 = _mm_set1_pd(0.00000001);    \
    const __m128d _TEN = _mm_set1_pd(10.);  \
    const __m128d _ADD = _mm_set1_pd(0.00000000000001); \
    const __m128d _MASK0 = _mm_set1_pd(4503599627370496.);  \
    const __m128d _MASK1 = _mm_castsi128_pd(_mm_set_epi32(0x000fffff, 0xffffffff, 0x000fffff, 0xffffffff));    \
    const __m128d _ONE = _mm_set1_pd(1.);   \
    const __m128i _CMP0 = _mm_set_epi32(0, 999999999, 0, 999999999);   \
    const __m128i _CMP1 = _mm_set_epi32(0, 1000000000, 0, 1000000000); \
    const __m128i _CMP2 = _mm_set_epi32(0, 1, 0, 1);   \
    const __m128d _SHIFT0 = _mm_set_pd(16777216., 1.);   \
    const __m128d _SHIFT1 = _mm_set_pd(1., 256.);    \
    const __m128d _SHIFT2 = _mm_set_pd(256., 65536.);    \
    const __m128d _SHIFT3 = _mm_set_pd(65536., 16777216.);   \
    \
    register __m128i _r0i, _r1i, _r2i, _r3i, _r4i, _r5i; \
    register __m128d _r0d, _r1d, _r2d, _r3d;   \
    u64_t _i0, _i1;  \
    register __m128i _o0, _o1, _o2;   \
    \
    /*  Load 2 words    */  \
    /*  _r1i = 1234567895987654321  */  \
    _r0i = ((const __m128i*)(A))[0];    \
    _r1i = _mm_srli_epi64(_r0i, 12); \
    \
    /*  Convert "_r1i" to floating-point.   */  \
    _r1d = _mm_castsi128_pd(_r1i);  \
    _r1d = _mm_or_pd(_r1d, _MASK0);  \
    _r1d = _mm_sub_pd(_r1d, _MASK0); \
    \
    /*  Scale it by 4096 * 10^-9.   */  \
    /*  This also counters the previous right-shift by 12 bits. */  \
    _r1d = _mm_mul_pd(_r1d, _SCALE0);    \
    \
    /*  Purturb the value down by 1. Don't let it go negative.  */  \
    _r1d = _mm_sub_pd(_r1d, _ONE);   \
    _r1d = _mm_max_pd(_r1d, _mm_setzero_pd());   \
    \
    /*  Convert back to integer.    */  \
    _r1d = _mm_add_pd(_r1d, _MASK0); \
    _r1d = _mm_and_pd(_r1d, _MASK1); \
    _r1i = _mm_castpd_si128(_r1d);  \
    \
    \
    \
    /*  At this point, "_r1i" holds the top 10 digits of each word. */  \
    /*  _r1i = 1234567895---------  */  \
    /*  Note that this result may not be exact. (It may be off by 1.)   */  \
    \
    \
    \
    /*  Multiply "_r1i" by 10^9. Store into "_r2i". */  \
    _i0 = _mm_cvtsi128_si64(_r1i);  \
    _i1 = _mm_extract_epi64(_r1i, 1);    \
    _i0 = _i0 * 1000000000ul;   \
    _i1 = _i1 * 1000000000ul;   \
    _r2i = _mm_cvtsi64_si128(_i0);  \
    _r2i = _mm_insert_epi64(_r2i, _i1, 1);    \
    \
    /*  Subtract from "_r0i". This will expose the bottom 9 digits. */  \
    /*  _r0i = ----------987654321  */  \
    _r0i = _mm_sub_epi64(_r0i, _r2i);    \
    \
    /*  Verify the split. Make corrections. */  \
    _r2i = _mm_cmpgt_epi32(_r0i, _CMP0); \
    _r3i = _mm_and_si128(_r2i, _CMP1);   \
    _r2i = _mm_and_si128(_r2i, _CMP2);   \
    _r0i = _mm_sub_epi64(_r0i, _r3i);    \
    _r1i = _mm_add_epi64(_r1i, _r2i);    \
    \
    \
    \
    /*  At this point:  */  \
    /*  "_r0i" holds the bottom 9 digits of each word.  */  \
    /*  "_r1i" holds the top 10 digits of each word.    */  \
    /*  _r0i = ----------987654321  */  \
    /*  _r1i = 1234567895---------  */  \
    /*  This result is provably exact.  */  \
    \
    \
    \
    /*  Convert "_r0i" and "_r1i" to floating-point.    */  \
    _r1d = _mm_castsi128_pd(_r1i);  \
    _r0d = _mm_castsi128_pd(_r0i);  \
    _r1d = _mm_or_pd(_r1d, _MASK0);  \
    _r0d = _mm_or_pd(_r0d, _MASK0);  \
    _r1d = _mm_sub_pd(_r1d, _MASK0); \
    _r0d = _mm_sub_pd(_r0d, _MASK0); \
    \
    \
    \
    /*  The following pattern will repeat several times.    */  \
    \
    /*  Scale so that all but the first digit is just below the decimal place.  */  \
    /*  _r1d = 1.234567895      */  \
    /*  _r0d = 9.87654321       */  \
    _r1d = _mm_mul_pd(_r1d, _SCALE2);    \
    _r0d = _mm_mul_pd(_r0d, _SCALE3);    \
    \
    /*  Purturb up. This will force rounding in the desired direction.  */  \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    \
    /*  Keep the first digit.   */  \
    /*  _r3d = 1.               */  \
    /*  _r2d = 9.               */  \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    \
    /*  Subtract to eliminate that first digit. */  \
    /*  _r1d = 0.234567895      */  \
    /*  _r0d = 0.87654321       */  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    \
    /*  Put into output vector.   */  \
    _r3d = _mm_mul_pd(_r3d, _SHIFT0);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT2);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _o0 = _mm_shuffle_epi32(_r3i, 168);  \
    _o1 = _mm_shuffle_epi32(_r3i, 169);  \
    _r4i = _mm_shuffle_epi32(_r2i, 138); \
    _r2i = _mm_shuffle_epi32(_r2i, 106); \
    _o0 = _mm_or_si128(_o0, _r4i);   \
    _o1 = _mm_or_si128(_o1, _r2i);   \
    \
    /*  2nd Digit   */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r0d = _mm_mul_pd(_r0d, _TEN);   \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT1);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT3);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 168); \
    _r4i = _mm_shuffle_epi32(_r2i, 138); \
    _r3i = _mm_shuffle_epi32(_r3i, 166); \
    _r2i = _mm_shuffle_epi32(_r2i, 106); \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    _o0 = _mm_or_si128(_o0, _r4i);   \
    _o1 = _mm_or_si128(_o1, _r2i);   \
    \
    /*  3rd Digit   */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r0d = _mm_mul_pd(_r0d, _TEN);   \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT2);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT0);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 168); \
    _r4i = _mm_shuffle_epi32(_r2i, 42);  \
    _r3i = _mm_shuffle_epi32(_r3i, 166); \
    _r2i = _mm_shuffle_epi32(_r2i, 106); \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    _o0 = _mm_or_si128(_o0, _r4i);   \
    _o1 = _mm_or_si128(_o1, _r2i);   \
    \
    /*  4th Digit   */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r0d = _mm_mul_pd(_r0d, _TEN);   \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT3);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT1);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 168); \
    _o2 = _mm_shuffle_epi32(_r2i, 169);  \
    _r3i = _mm_shuffle_epi32(_r3i, 166); \
    _r2i = _mm_shuffle_epi32(_r2i, 42);  \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    _o0 = _mm_or_si128(_o0, _r2i);   \
    \
    /*  5th Digit   */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r0d = _mm_mul_pd(_r0d, _TEN);   \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT0);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT2);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 162); \
    _r4i = _mm_shuffle_epi32(_r2i, 42);  \
    _r3i = _mm_shuffle_epi32(_r3i, 166); \
    _r2i = _mm_shuffle_epi32(_r2i, 169); \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    _o2 = _mm_or_si128(_o2, _r2i);   \
    _o0 = _mm_or_si128(_o0, _r4i);   \
    \
    /*  6th Digit   */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r0d = _mm_mul_pd(_r0d, _TEN);   \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT1);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT3);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 162); \
    _r4i = _mm_shuffle_epi32(_r2i, 42);  \
    _r3i = _mm_shuffle_epi32(_r3i, 154); \
    _r2i = _mm_shuffle_epi32(_r2i, 169); \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    _o2 = _mm_or_si128(_o2, _r2i);   \
    _o0 = _mm_or_si128(_o0, _r4i);   \
    \
    /*  7th Digit   */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r0d = _mm_mul_pd(_r0d, _TEN);   \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT2);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT0);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 162); \
    _r4i = _mm_shuffle_epi32(_r2i, 168); \
    _r3i = _mm_shuffle_epi32(_r3i, 154); \
    _r2i = _mm_shuffle_epi32(_r2i, 169); \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o2 = _mm_or_si128(_o2, _r2i);   \
    _o1 = _mm_or_si128(_o1, _r4i);   \
    \
    /*  8th Digit   */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r0d = _mm_mul_pd(_r0d, _TEN);   \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT3);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT1);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 162); \
    _r4i = _mm_shuffle_epi32(_r2i, 168); \
    _r3i = _mm_shuffle_epi32(_r3i, 154); \
    _r2i = _mm_shuffle_epi32(_r2i, 166); \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o2 = _mm_or_si128(_o2, _r2i);   \
    _o1 = _mm_or_si128(_o1, _r4i);   \
    \
    /*  9th Digit   */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r0d = _mm_mul_pd(_r0d, _TEN);   \
    _r1d = _mm_add_pd(_r1d, _ADD);   \
    _r0d = _mm_add_pd(_r0d, _ADD);   \
    _r3d = _mm_floor_pd(_r1d);  \
    _r2d = _mm_floor_pd(_r0d);  \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r0d = _mm_sub_pd(_r0d, _r2d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT0);    \
    _r2d = _mm_mul_pd(_r2d, _SHIFT2);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r2i = _mm_cvttpd_epi32(_r2d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 138); \
    _r4i = _mm_shuffle_epi32(_r2i, 168); \
    _r3i = _mm_shuffle_epi32(_r3i, 154); \
    _r2i = _mm_shuffle_epi32(_r2i, 166); \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o2 = _mm_or_si128(_o2, _r2i);   \
    _o1 = _mm_or_si128(_o1, _r4i);   \
    \
    /*  10th Digit  */  \
    _r1d = _mm_mul_pd(_r1d, _TEN);   \
    _r3d = _mm_round_pd(_r1d, 0);    \
    _r1d = _mm_sub_pd(_r1d, _r3d);   \
    _r3d = _mm_mul_pd(_r3d, _SHIFT1);    \
    _r3i = _mm_cvttpd_epi32(_r3d);  \
    _r5i = _mm_shuffle_epi32(_r3i, 138); \
    _r3i = _mm_shuffle_epi32(_r3i, 106); \
    _o0 = _mm_or_si128(_o0, _r5i);   \
    _o1 = _mm_or_si128(_o1, _r3i);   \
    \
    /*  Store to destination.   */  \
    _mm_storeu_si128((__m128i*)(T) + 0, _o0);    \
    _mm_storeu_si128((__m128i*)(T) + 1, _o1);    \
    _i0 = _mm_cvtsi128_si64(_o2);   \
    *((u32_t*)(T) +  8) = (u32_t)_i0;   \
    *((u16_t*)(T) + 18) = (u16_t)(_i0 >> 32);   \
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
