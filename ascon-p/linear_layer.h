#pragma once

#include <_internal/state.h>

#if defined(__RV32__) && defined(RV_HAVE_ROR)
    #if !defined(__BI32__)
        #error rv32 always uses bit interleaving!
    #endif

    #define RV_ROR64(dst_lo, dst_hi, src_lo, src_hi, n) {           \
        if ((n) % 2 == 0) {                                         \
            __asm__ volatile (                                      \
                "rori %0, %2, %c4;"                                 \
                "rori %1, %3, %c4;"                                 \
                : "+r"(dst_lo), "+r"(dst_hi),                       \
                  "+r"(src_lo), "+r"(src_hi)                        \
                : "i"((n) / 2)                                      \
            );                                                      \
        } else {                                                    \
            __asm__ volatile (                                      \
                "rori %0, %3, %c4;"                                 \
                "rori %1, %2, %c5;"                                 \
                : "+r"(dst_lo), "+r"(dst_hi),                       \
                  "+r"(src_lo), "+r"(src_hi)                        \
                : "i"(((n) - 1) / 2), "i"(((n) + 1) / 2)            \
            );                                                      \
        }                                                           \
    }

    #define RV_LINEAR(dst_lo, dst_hi, src_lo, src_hi, r0, r1, t0, t1) { \
        RV_ROR64(t0, t1, src_lo, src_hi, r1);                           \
        __asm__ volatile (                                              \
            "xor %0, %2, %4;"                                           \
            "xor %1, %3, %5;"                                           \
            : "+r"(dst_lo), "+r"(dst_hi),                               \
              "+r"(src_lo), "+r"(src_hi),                               \
              "+r"(t0), "+r"(t1)                                        \
        );                                                              \
                                                                        \
        RV_ROR64(t0, t1, dst_lo, dst_hi, r0);                           \
        __asm__ volatile (                                              \
            "xor %0, %2, %4;"                                           \
            "xor %1, %3, %5;"                                           \
            : "+r"(dst_lo), "+r"(dst_hi),                               \
              "+r"(src_lo), "+r"(src_hi),                               \
              "+r"(t0), "+r"(t1)                                        \
        );                                                              \
    }

#elif defined(__RV32__)
    #define NEED_GENERIC_LINEAR_LAYER
    #if !defined(__BI32__)
        #error rv32 always uses bit interleaving!
    #endif

    #define RV_ROR64(dst_lo, dst_hi, src_lo, src_hi, n) {   \
        dst_lo = ((n) % 2)                                  \
            ? generic_ror32(src_hi, ((n) - 1) / 2)          \
            : generic_ror32(src_lo, (n) / 2);               \
        dst_hi = ((n) % 2)                                  \
            ? generic_ror32(src_lo, ((n) + 1) / 2)          \
            : generic_ror32(src_hi, (n) / 2);               \
    }

    #define RV_LINEAR(dst_lo, dst_hi, src_lo, src_hi, r0, r1, t0, t1) { \
        RV_ROR64(t0, t1, src_lo, src_hi, r1);                           \
        dst_lo = src_lo ^ t0;                                           \
        dst_hi = src_hi ^ t1;                                           \
                                                                        \
        RV_ROR64(t0, t1, dst_lo, dst_hi, r0);                           \
        dst_lo = src_lo ^ t0;                                           \
        dst_hi = src_hi ^ t1;                                           \
    }

#elif defined(__RV64__) && defined(RV_HAVE_ROR)

    #define RV_LINEAR(dst, src, r0, r1, t0) {   \
        __asm__ volatile (                      \
            "ror %0, %1, %c3;"                  \
            "xor %0, %1, %0;"                   \
            "ror %2, %1, %c4;"                  \
            "xor %0, %0, %2;"                   \
            : "+r"(dst), "+r"(src), "+r"(t0)    \
            : "i"(r0), "i"(r1)                  \
        );                                      \
    }

#elif defined(__RV64__)
    #define NEED_GENERIC_LINEAR_LAYER

    #define RV_LINEAR(dst, src, r0, r1, t0) {                               \
        dst = generic_ror64(src, r0) ^ generic_ror64(src, r1) ^ (src);      \
    }

#else

    #error linear layer unimplemented for architecture other than RISC-V

#endif

#if defined(TESTING) || defined(NEED_GENERIC_LINEAR_LAYER)

    #if defined(__BI32__)

        static inline uint32_t generic_ror32(uint32_t x, int n) {
            return x >> n | x << (-n & 31);
        }

        static inline uint64_t generic_ror64(uint64_t x, int n) {
            word_t b, a = { .x = x };
            b.w[0] = (n % 2) ? generic_ror32(a.w[1], (n - 1) / 2) : generic_ror32(a.w[0], n / 2);
            b.w[1] = (n % 2) ? generic_ror32(a.w[0], (n + 1) / 2) : generic_ror32(a.w[1], n / 2);
            return b.x;
        }

        static inline void generic_linear_layer(state_t* s) {
            state_t tmp = *s;
            s->x[2] = tmp.x[2] ^ generic_ror64(tmp.x[2], 6 - 1);
            s->x[3] = tmp.x[3] ^ generic_ror64(tmp.x[3], 17 - 10);
            s->x[4] = tmp.x[4] ^ generic_ror64(tmp.x[4], 41 - 7);
            s->x[0] = tmp.x[0] ^ generic_ror64(tmp.x[0], 28 - 19);
            s->x[1] = tmp.x[1] ^ generic_ror64(tmp.x[1], 61 - 39);
            s->x[2] = tmp.x[2] ^ generic_ror64(s->x[2], 1);
            s->x[3] = tmp.x[3] ^ generic_ror64(s->x[3], 10);
            s->x[4] = tmp.x[4] ^ generic_ror64(s->x[4], 7);
            s->x[0] = tmp.x[0] ^ generic_ror64(s->x[0], 19);
            s->x[1] = tmp.x[1] ^ generic_ror64(s->x[1], 39);
        }

    #else

        static inline uint64_t generic_ror64(uint64_t x, int n) {
            return x >> n | x << (-n & 63);
        }

        static inline void generic_linear_layer(state_t* s) {
            state_t tmp = *s;
            s->x[0] = tmp.x[0] ^ generic_ror64(tmp.x[0], 19) ^ generic_ror64(tmp.x[0], 28);
            s->x[1] = tmp.x[1] ^ generic_ror64(tmp.x[1], 61) ^ generic_ror64(tmp.x[1], 39);
            s->x[2] = tmp.x[2] ^ generic_ror64(tmp.x[2], 1)  ^ generic_ror64(tmp.x[2], 6);
            s->x[3] = tmp.x[3] ^ generic_ror64(tmp.x[3], 10) ^ generic_ror64(tmp.x[3], 17);
            s->x[4] = tmp.x[4] ^ generic_ror64(tmp.x[4], 7)  ^ generic_ror64(tmp.x[4], 41);
        }

    #endif

#endif // #if defined(TESTING) || defined(NEED_GENERIC_LINEAR_LAYER)
