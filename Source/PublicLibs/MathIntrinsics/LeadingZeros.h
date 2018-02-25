/* LeadingZeros.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/26/2014
 * Last Modified    : 07/26/2014
 * 
 * 
 *      Return false if no set bits are found.
 * 
 */

#pragma once
#ifndef ymp_Intrinsics_LeadingZeros_H
#define ymp_Intrinsics_LeadingZeros_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  32-bit
#if 0
#elif _MSC_VER
#include <intrin.h>
namespace ymp{
namespace Intrinsics{
    YM_FORCE_INLINE bool leading_zeros(ukL_t& zeros, u32_t x){
        unsigned long tmp;
        char ret = _BitScanReverse(&tmp, x);
        zeros = 31 - (ukL_t)tmp;
        return ret != 0;
    }
}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif __GNUC__
namespace ymp{
namespace Intrinsics{
    YM_FORCE_INLINE bool leading_zeros(ukL_t& zeros, u32_t x){
        zeros = (ukL_t)__builtin_clz(x);
        return x != 0;
    }
}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#else
namespace ymp{
namespace Intrinsics{
    YM_FORCE_INLINE bool leading_zeros(ukL_t& zeros, u32_t x){
        const u32_t MASK = 0x80000000ull;

        if (x == 0){
            return false;
        }

        zeros = 0;
        while ((x & MASK) == 0){
            x <<= 1;
            zeros++;
        }

        return true;
    }
}
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  64-bit
#if 0
#elif _WIN64 && _MSC_VER
#include <intrin.h>
namespace ymp{
namespace Intrinsics{
    YM_FORCE_INLINE bool leading_zeros(ukL_t& zeros, u64_t x){
        unsigned long tmp;
        char ret = _BitScanReverse64(&tmp, x);
        zeros = 63 - (ukL_t)tmp;
        return ret != 0;
    }
}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#elif __GNUC__
namespace ymp{
namespace Intrinsics{
    YM_FORCE_INLINE bool leading_zeros(ukL_t& zeros, u64_t x){
        zeros = (ukL_t)__builtin_clzll(x);
        return x != 0;
    }
}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#else
namespace ymp{
namespace Intrinsics{
    YM_FORCE_INLINE bool leading_zeros(ukL_t& zeros, u64_t x){
        const u64_t MASK = 0x8000000000000000ull;

        if (x == 0){
            return false;
        }

        zeros = 0;
        while ((x & MASK) == 0){
            x <<= 1;
            zeros++;
        }

        return true;
    }
}
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
