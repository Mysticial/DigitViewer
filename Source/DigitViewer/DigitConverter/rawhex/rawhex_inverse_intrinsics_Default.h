/* rawhex_inverse_intrinsics_Default.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/30/2011
 * Last Modified    : 01/24/2016
 * 
 */

#ifndef _ydv_rawhex_inverse_intrinsics_Default_H
#define _ydv_rawhex_inverse_intrinsics_Default_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace DigitViewer{
namespace rawhex{
    using namespace ymp;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE char convert_hex1_inverse_Default(char str[1]){
    char bad;
    char r0 = str[0];
    bad  = r0 < '0' || r0 > 'f' || (r0 > '9' && r0 < 'a')
        ? 1
        : 0;
    r0 = r0 >= 'a' ? r0 - 39 : r0;
    r0 -= '0';
    str[0] = r0;
    return bad;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE u32_t convert_hex4_inverse_Default32(char str[4]){
    u32_t* T = (u32_t*)str;

    u32_t bad;
    u32_t a0, b0, c0;

    a0 = T[0];

    b0 = a0 & 0x80808080ull;
    bad = b0;

    b0 = a0 + 0x50505050ull;
    b0 = ~b0;
    b0 &= 0x80808080ull;
    bad |= b0;

    b0 = a0 + 0x19191919ull;
    b0 &= 0x80808080ull;
    bad |= b0;

    c0 = a0 + 0x46464646ull;
    c0 &= 0x80808080ull;

    b0 = a0 + 0x1f1f1f1full;
    b0 = ~b0;
    b0 &= 0x80808080ull;
    b0 &= c0;
    bad |= b0;

    c0 >>= 7;
    c0 *= 39;
    a0 -= c0;

    a0 -= 0x30303030ull;
    T[0] = a0;

    return bad;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
