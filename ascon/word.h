#pragma once

#include <_internal/common.h>
#include "interleaving.h"

static inline_opt uint64_t PAD(int i) {
    #if defined(__BI32__)
        return ((uint64_t)((uint32_t)0x08 << (28 - 4 * i)) << 32);
    #else
        return (uint64_t)0x80 << (56 - 8 * i);
    #endif 
}

static inline_opt uint64_t inbound_word(uint64_t x)
{
    #if defined(__BI32__)
        return deinterleave32(x);
    #else
        return U64BIG(x);
    #endif
}

static inline_opt uint64_t outbound_word(uint64_t x)
{
    #if defined(__BI32__)
        return interleave32(x);
    #else
        return U64BIG(x);
    #endif
}

static inline_opt uint64_t LOAD_N(const uint8_t* bytes, int n)
{
    uint64_t x = 0;
    memcpy(&x, bytes, n);
    return inbound_word(x);
}

static inline_opt void STORE_N(uint8_t* bytes, uint64_t x, int n)
{
    x = outbound_word(x);
    memcpy(bytes, &x, n);
}

static inline_opt uint64_t LOAD_64(const uint8_t* bytes)
{
    return inbound_word(*(uint64_t*)bytes);
}

static inline_opt void STORE_64(uint8_t* bytes, uint64_t x)
{
    *(uint64_t*)bytes = outbound_word(x);
}    

/* set byte in 64-bit Ascon word */
#define SETBYTE(b, i) ((uint64_t)(b) << (56 - 8 * (i)))

static inline_opt uint64_t CLEAR_N(uint64_t x, int n) {
    #if defined(__BI32__)
        /* undefined for n == 0 */
        uint32_t mask = 0xffffffffull >> (4 * n);
        return x & ((uint64_t)mask << 32 | mask);
    #else
        for (int i = 0; i < n; ++i) x &= ~SETBYTE(0xff, i);
        return x;
    #endif
}
