/* ChecksumHash.h - Checksum Hashing
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 12/26/2010
 * Last Modified    : 07/14/2014
 * 
 *      The prime number used for this hash is 2^61 - 1.
 * 
 *  This file is intentionally lightweight since it's used in a lot of headers.
 * 
 */

#pragma once
#ifndef ymp_ChecksumHash_H
#define ymp_ChecksumHash_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/Types.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class hash_t{
public:
    static const u64_t PRIME = 0x1fffffffffffffffull;

public:
    YM_FORCE_INLINE hash_t() = default;
    YM_FORCE_INLINE hash_t(int x);
    YM_FORCE_INLINE hash_t(u32_t x);
    YM_FORCE_INLINE hash_t(u64_t x);

public:
    YM_FORCE_INLINE u64_t value() const{
        return m_hash;
    }

public:
    friend bool operator==(hash_t a, hash_t b);
    friend bool operator!=(hash_t a, hash_t b);

    friend hash_t operator+(hash_t a);
    friend hash_t operator-(hash_t a);

    friend hash_t operator+(hash_t a, hash_t b);
    friend hash_t operator-(hash_t a, hash_t b);
    friend hash_t operator*(hash_t a, hash_t b);
    friend hash_t operator^(hash_t a, uiL_t pow);
    friend hash_t operator^(hash_t a, siL_t pow);

    void operator+=(hash_t b);
    void operator-=(hash_t b);
    void operator*=(hash_t b);
    void operator^=(uiL_t pow);
    void operator^=(siL_t pow);

    template <typename wtype>
    static hash_t word_power(siL_t pow);

private:
    u64_t m_hash;

    friend hash_t bp_hash(hash_t hash_in, const u32_t* T, upL_t L);
    friend hash_t bp_hash(hash_t hash_in, const u64_t* T, upL_t L);
    friend hash_t bp_hash_radix(hash_t hash_in, u32_t radix, const u32_t* T, upL_t L);
    friend hash_t bp_hash_radix(hash_t hash_in, u64_t radix, const u64_t* T, upL_t L);

    YM_FORCE_INLINE hash_t(u64_t x, void*) : m_hash(x) {}
    static YM_FORCE_INLINE u64_t reduce(u64_t L, u64_t H);
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Implementations
YM_FORCE_INLINE hash_t::hash_t(int x)
    : m_hash(x >= 0 ? x : x + PRIME)
{}
YM_FORCE_INLINE hash_t::hash_t(u32_t x)
    : m_hash(x)
{}
YM_FORCE_INLINE hash_t::hash_t(u64_t x)
    : m_hash(x)
{
    x >>= 61;
    m_hash &= PRIME;
    m_hash += x;
    if (m_hash >= PRIME){
        m_hash -= PRIME;
    }
}
YM_FORCE_INLINE u64_t hash_t::reduce(u64_t L, u64_t H){
    H <<= 3;
    H |= (u64_t)L >> 61;
    L &= 0x1fffffffffffffffull;
    L += H;
    H = L - 0x1fffffffffffffffull;
    return L >= 0x1fffffffffffffffull ? H : L;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE bool operator==(hash_t a, hash_t b){
    return a.m_hash == b.m_hash;
}
YM_FORCE_INLINE bool operator!=(hash_t a, hash_t b){
    return a.m_hash != b.m_hash;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE hash_t operator+(hash_t a){
    return a;
}
YM_FORCE_INLINE hash_t operator-(hash_t a){
    return a.m_hash == 0 ? 0 : hash_t::PRIME - a.m_hash;
}
YM_FORCE_INLINE hash_t operator+(hash_t a, hash_t b){
    a += b;
    return a;
}
YM_FORCE_INLINE hash_t operator-(hash_t a, hash_t b){
    a -= b;
    return a;
}
YM_FORCE_INLINE hash_t operator*(hash_t a, hash_t b){
    a *= b;
    return a;
}
YM_FORCE_INLINE hash_t operator^(hash_t a, uiL_t pow){
    a ^= pow;
    return a;
}
YM_FORCE_INLINE hash_t operator^(hash_t a, siL_t pow){
    a ^= pow;
    return a;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_FORCE_INLINE void hash_t::operator+=(hash_t b){
    m_hash += b.m_hash;
    if (m_hash >= PRIME){
        m_hash -= PRIME;
    }
}
YM_FORCE_INLINE void hash_t::operator-=(hash_t b){
    m_hash -= b.m_hash;
    if ((s64_t)m_hash < 0){
        m_hash += PRIME;
    }
}
YM_FORCE_INLINE void hash_t::operator^=(siL_t pow){
    if (pow < 0){
        pow += PRIME - 1;
    }
    *this ^= (uiL_t)pow;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
