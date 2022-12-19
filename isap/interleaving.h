#pragma once

#if defined(__RV32__) && defined(HAVE_ZBKB)

    #define RV32_LOAD_SWAP_DEINTERLEAVE_32(lo_out, hi_out, t0, word64_ptr) { \
        hi_out = *(((uint32_t*)(word64_ptr)) + 0);                           \
        t0 =     *(((uint32_t*)(word64_ptr)) + 1);                           \
                                                                             \
        __asm__ volatile (                                                   \
            "rev8 %0, %0;"                                                   \
            "rev8 %1, %1;"                                                   \
            "unzip %0, %0;"                                                  \
            "unzip %1, %1;"                                                  \
            "pack %2, %0, %1;"                                               \
            "srli %0, %0, 16;"                                               \
            "srli %1, %1, 16;"                                               \
            "pack %1, %0, %1;"                                               \
            : "+r"(t0), "+r"(hi_out), "+r"(lo_out)                           \
        );                                                                   \
    }

    #define RV32_SWAP_INTERLEAVE_32(lo_out, hi_out, lo, hi, t0) {   \
        lo_out = lo;                                                \
        hi_out = hi;                                                \
        __asm__ volatile (                                          \
            "pack %0, %2, %1;"                                      \
            "srli %1, %1, 16;"                                      \
            "srli %2, %2, 16;"                                      \
            "pack %1, %2, %1;"                                      \
            "zip %0, %0;"                                           \
            "zip %1, %1;"                                           \
            "rev8 %0, %0;"                                          \
            "rev8 %1, %1;"                                          \
            : "+r"(t0), "+r"(hi_out), "+r"(lo_out)                  \
        );                                                          \
        lo_out = hi_out;                                            \
        hi_out = t0;                                                \
    }

#elif defined(__RV32__)
    #define NEED_GENERIC_LOAD_SWAP_DEINTERLEAVE_32
    #define RV32_LOAD_SWAP_DEINTERLEAVE_32(lo_out, hi_out, t0, word64_ptr)  \
        GENERIC_LOAD_SWAP_DEINTERLEAVE_32(lo_out, hi_out, word64_ptr)

    #define NEED_GENERIC_SWAP_INTERLEAVE_32
    #define RV32_SWAP_INTERLEAVE_32(lo_out, hi_out, lo, hi, t0)  \
        GENERIC_SWAP_INTERLEAVE_32(lo_out, hi_out, lo, hi)
#else
    #define NEED_GENERIC_LOAD_SWAP_DEINTERLEAVE_32
    #define NEED_GENERIC_SWAP_INTERLEAVE_32
#endif

#if defined(NEED_GENERIC_LOAD_SWAP_DEINTERLEAVE_32)

    #define LOAD_SWAP_DEINTERLEAVE_32_impl(lo_out, hi_out, word64_ptr,      \
            r0, r1, lo, hi) {                                               \
        hi = __builtin_bswap32(*(((uint32_t*)(word64_ptr)) + 0));           \
        lo = __builtin_bswap32(*(((uint32_t*)(word64_ptr)) + 1));           \
                                                                            \
        r0 = (lo ^ (lo >> 1)) & 0x22222222, lo ^= r0 ^ (r0 << 1);           \
        r0 = (lo ^ (lo >> 2)) & 0x0C0C0C0C, lo ^= r0 ^ (r0 << 2);           \
        r0 = (lo ^ (lo >> 4)) & 0x00F000F0, lo ^= r0 ^ (r0 << 4);           \
        r0 = (lo ^ (lo >> 8)) & 0x0000FF00, lo ^= r0 ^ (r0 << 8);           \
        r1 = (hi ^ (hi >> 1)) & 0x22222222, hi ^= r1 ^ (r1 << 1);           \
        r1 = (hi ^ (hi >> 2)) & 0x0C0C0C0C, hi ^= r1 ^ (r1 << 2);           \
        r1 = (hi ^ (hi >> 4)) & 0x00F000F0, hi ^= r1 ^ (r1 << 4);           \
        r1 = (hi ^ (hi >> 8)) & 0x0000FF00, hi ^= r1 ^ (r1 << 8);           \
                                                                            \
        lo_out = (lo & 0x0000FFFF) | (hi << 16);                            \
        hi_out = (lo >> 16) | (hi & 0xFFFF0000);                            \
    }

    #define GENERIC_LOAD_SWAP_DEINTERLEAVE_32(lo_out, hi_out, word64_ptr) { \
        uint32_t r0, r1, lo, hi;                                            \
        LOAD_SWAP_DEINTERLEAVE_32_impl(                                     \
            lo_out, hi_out, word64_ptr, r0, r1, lo, hi                      \
        );                                                                  \
    }

    #define GENERIC_SWAP_INTERLEAVE_32(lo, hi, lo_in, hi_in) {      \
        uint32_t r0, r1;                                            \
        lo = (lo_in & 0x0000FFFF) | (hi_in << 16);                  \
        hi = (lo_in >> 16) | (hi_in & 0xFFFF0000);                  \
        r0 = (lo ^ (lo >> 8)) & 0x0000FF00, lo ^= r0 ^ (r0 << 8);   \
        r0 = (lo ^ (lo >> 4)) & 0x00F000F0, lo ^= r0 ^ (r0 << 4);   \
        r0 = (lo ^ (lo >> 2)) & 0x0C0C0C0C, lo ^= r0 ^ (r0 << 2);   \
        r0 = (lo ^ (lo >> 1)) & 0x22222222, lo ^= r0 ^ (r0 << 1);   \
        r1 = (hi ^ (hi >> 8)) & 0x0000FF00, hi ^= r1 ^ (r1 << 8);   \
        r1 = (hi ^ (hi >> 4)) & 0x00F000F0, hi ^= r1 ^ (r1 << 4);   \
        r1 = (hi ^ (hi >> 2)) & 0x0C0C0C0C, hi ^= r1 ^ (r1 << 2);   \
        r1 = (hi ^ (hi >> 1)) & 0x22222222, hi ^= r1 ^ (r1 << 1);   \
        r0 = hi; hi = __builtin_bswap32(lo); lo = __builtin_bswap32(r0); \
    }

#endif
