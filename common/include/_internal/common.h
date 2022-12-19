#pragma once

#include <stdint.h>

#define forceinline inline __attribute__((__always_inline__))
#if defined(OPT_SPEED)
    #define inline_opt forceinline
#else
    #define inline_opt
#endif

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define U64BIG(x) (x)
#elif defined(__RV64__) && defined(HAVE_REV8)
    static forceinline uint64_t U64BIG(uint64_t x) {
        uint64_t result;
        __asm__ volatile (
            "rev8 %0, %1;"
            : "=r"(result) : "r"(x)
        );
        return result;
    }
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #if defined(TESTING)
        // No need to optimize this, since it's only used for testing!
        #define U64BIG(x) __builtin_bswap64(x)
    #endif
#else
    #error neither little nor big endian macros defined!
#endif

void* memcpy(void* dest, const void* src, unsigned long n);
