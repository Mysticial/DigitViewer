/* DigitViewer_RawToI64_x64_AVX512BW.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/10/2018
 * Last Modified    : 02/10/2018
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <algorithm>
#include "PublicLibs/BasicLibs/Alignment/AlignmentTools.h"
#include "i64ToDec_Default.h"
#include "i64ToHex_LittleEndian.h"
#include "i64ToDec_LittleEndian.h"
#include "i64ToHex_x64_AVX512BW.h"
#include "i64ToDec_x64_AVX512BW.h"
namespace DigitViewer2{
namespace RawToCompressed{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE bool hex_to_i64(u64_t* T, const char* raw, upL_t words){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<64>(T);
        block = std::min(block, words);
        bad |= hex_to_i64_LittleEndian(T, (const u64_t*)raw, block);
        T     += block;
        raw   += block * 16;
        words -= block;
    }
    {
        upL_t blocks = words / 8;
        bad |= hex_to_i64_u8_x64_AVX512BW((__m512i*)T, (const __m512i*)raw, blocks);
        T     += blocks * 8;
        raw   += blocks * 8 * 16;
        words -= blocks * 8;
    }
    bad |= hex_to_i64_LittleEndian(T, (const u64_t*)raw, words);
    return bad;
}
YM_NO_INLINE void i64_to_hex(char* raw, const u64_t* T, upL_t words){
    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<64>(T);
        block = std::min(block, words);
        i64_to_hex_LittleEndian((u64_t*)raw, T, block);
        T     += block;
        raw   += block * 16;
        words -= block;
    }
    {
        upL_t blocks = words / 8;
        i64_to_hex_u8_x64_AVX512BW((__m512i*)raw, (__m256i*)T, blocks);
        T     += blocks * 8;
        raw   += blocks * 8 * 16;
        words -= blocks * 8;
    }
    return i64_to_hex_LittleEndian((u64_t*)raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE bool dec_to_i64(u64_t* T, const char* raw, upL_t words){
    bool bad = false;

    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<64>(T);
        block = std::min(block, words);
        bad |= dec_to_i64_LittleEndian(T, raw, block);
        T     += block;
        raw   += block * 19;
        words -= block;
    }
    {
        upL_t blocks = words / 8;
        bad |= dec_to_i64_u8_x64_AVX512BW((__m512i*)T, raw, blocks);
        T     += blocks * 8;
        raw   += blocks * 8 * 19;
        words -= blocks * 8;
    }
    bad |= dec_to_i64_LittleEndian(T, raw, words);
    return bad;
}
YM_NO_INLINE void i64_to_dec(char* raw, const u64_t* T, upL_t words){
    //  Align
    {
        upL_t block = Alignment::ptr_to_aligned<64>(T);
        block = std::min(block, words);
        i64_to_dec_Default(raw, T, block);
        T     += block;
        raw   += block * 19;
        words -= block;
    }
    {
        upL_t blocks = words / 8;
        i64_to_dec_u8_x64_AVX512BW(raw, (__m512i*)T, blocks);
        T     += blocks * 8;
        raw   += blocks * 8 * 19;
        words -= blocks * 8;
    }
    i64_to_dec_Default(raw, T, words);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
