#pragma once

#include <_internal/common.h>

#if defined(__RV32__) && defined(__BI32__)

    #if defined(HAVE_ZBKB) // scalar crypto ISE is present

        #ifdef TESTING

            static inline uint32_t deinterleave16(uint32_t x) {
                __asm__ volatile (
                    "unzip %0, %0;"
                    : "+r"(x)
                );
                return x;
            }

            static inline uint32_t interleave16(uint32_t x) {
                __asm__ volatile (
                    "zip %0, %0;"
                    : "+r"(x)
                );
                return x;
            }

        #endif

        static inline_opt uint64_t deinterleave32(uint64_t in) {
            // flip endianness, step 1
            uint32_t lo_in = in >> 32;
            uint32_t hi = in;

            register uint32_t lo;
            __asm__ volatile (
                "rev8 %0, %0;"          // flip endianness, step 2
                "rev8 %1, %1;"          // flip endianness, step 3
                "unzip %0, %0;"         // deinterleave16()
                "unzip %1, %1;"         // deinterleave16()
                "pack %2, %0, %1;"      // pack
                "srli %0, %0, 16;"      // simulate 'packu' (which is no longer included
                "srli %1, %1, 16;"      // in Zbkb) with two shifts and 'pack'
                "pack %1, %0, %1;"      // <= 'packu'
                : "+r"(lo_in), "+r"(hi), "+r"(lo)
            );

            return (uint64_t)hi << 32 | lo;
        }

        static inline_opt uint64_t interleave32(uint64_t in) {
            uint32_t lo = in;
            uint32_t hi = in >> 32;

            register uint32_t lo_out;        
            __asm__ volatile (
                "pack %0, %2, %1;"      // 'pack'
                "srli %1, %1, 16;"      // simulate 'packu' (which is no longer included
                "srli %2, %2, 16;"      // in Zbkb) with two shifts and 'pack'
                "pack %1, %2, %1;"      // <= 'packu'
                "zip %0, %0;"           // interleave16()
                "zip %1, %1;"           // interleave16()
                "rev8 %0, %0;"          // flip endianness, step 1
                "rev8 %1, %1;"          // flip endianness, step 2
                : "+r"(lo_out), "+r"(hi)
                : "r"(lo)
            );

            // flip endianness, step 3
            return (uint64_t)lo_out << 32 | hi;
        }

    #else
        #define NEED_REF_INTERLEAVE
    #endif // scalar crypto ISE is present

    #if defined(TESTING) || defined(NEED_REF_INTERLEAVE)

        // the following generic_* functions have been sourced without
        // any changes from: https://github.com/ascon/ascon-c
        // they serve as a reference implementations for testing and
        // are used whenever an optimized variant is not available

        extern const uint32_t B[3];

        static inline uint32_t generic_deinterleave16(uint32_t x) {
            uint32_t t;
            t = (x ^ (x >> 1)) & B[0], x ^= t ^ (t << 1);
            t = (x ^ (x >> 2)) & B[1], x ^= t ^ (t << 2);
            t = (x ^ (x >> 4)) & B[2], x ^= t ^ (t << 4);
            t = (x ^ (x >> 8)) & 0xff00, x ^= t ^ (t << 8);
            return x;
        }

        static inline uint32_t generic_interleave16(uint32_t x) {
            uint32_t t;
            t = (x ^ (x >> 8)) & 0xff00, x ^= t ^ (t << 8);
            t = (x ^ (x >> 4)) & B[2], x ^= t ^ (t << 4);
            t = (x ^ (x >> 2)) & B[1], x ^= t ^ (t << 2);
            t = (x ^ (x >> 1)) & B[0], x ^= t ^ (t << 1);
            return x;
        }

        /* credit to Henry S. Warren, Hacker's Delight, Addison-Wesley, 2002 */
        static inline uint64_t generic_deinterleave32(uint64_t in) {
            in = U64BIG(in);
            uint32_t hi = in >> 32;
            uint32_t lo = in;
            uint32_t t0, t1, e, o;
            t0 = generic_deinterleave16(lo);
            t1 = generic_deinterleave16(hi);
            e = (t1 << 16) | (t0 & 0x0000FFFF);
            o = (t1 & 0xFFFF0000) | (t0 >> 16);
            return (uint64_t)o << 32 | e;
        }

        /* credit to Henry S. Warren, Hacker's Delight, Addison-Wesley, 2002 */
        static inline uint64_t generic_interleave32(uint64_t in) {
            uint32_t e = in;
            uint32_t o = in >> 32;
            uint32_t t0, t1, lo, hi;
            t0 = (o << 16) | (e & 0x0000FFFF);
            t1 = (o & 0xFFFF0000) | (e >> 16);
            lo = generic_interleave16(t0);
            hi = generic_interleave16(t1);
            return U64BIG((uint64_t)hi << 32 | lo);
        }

        #ifdef NEED_REF_INTERLEAVE

            #define interleave32 generic_interleave32
            #define deinterleave32 generic_deinterleave32

        #endif

        #ifdef TESTING

            #define interleave16_ref generic_interleave16
            #define interleave32_ref generic_interleave32
            #define deinterleave16_ref generic_deinterleave16
            #define deinterleave32_ref generic_deinterleave32

        #endif

    #endif

#endif // #if defined(__RV32__) && defined(__BI32__)
