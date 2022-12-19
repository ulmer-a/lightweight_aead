#pragma once

#include "s_box.h"
#include "linear_layer.h"

#if defined(__RV32__)

    #define RV32_PERM(n, x3_tmp_lo, x3_tmp_hi,                                  \
        x0_lo, x0_hi, x1_lo, x1_hi, x2_lo, x2_hi,                               \
        x3_lo, x3_hi, x4_lo, x4_hi, t0, t1, t2) {                               \
        for (int i = (12 - (n)) * 2; i < 24; i += 2)                            \
        {                                                                       \
            /* add the round constant */                                        \
            x2_lo ^= ascon_round_constants_bi[i];                               \
            x2_hi ^= ascon_round_constants_bi[i + 1];                           \
                                                                                \
            /* apply s-box */                                                   \
            RV_S_BOX(x0_lo, x1_lo, x2_lo, x3_lo, x4_lo, t0, t1, t2, x3_tmp_lo); \
            RV_S_BOX(x0_hi, x1_hi, x2_hi, x3_hi, x4_hi, t0, t1, t2, x3_tmp_hi); \
                                                                                \
            /* apply linear layer */                                            \
            RV_LINEAR(x0_lo, x0_hi, x2_lo, x2_hi, 19, 28 - 19, t0, t1);         \
            RV_LINEAR(x2_lo, x2_hi, x4_lo, x4_hi,  1,  6 -  1, t0, t1);         \
            RV_LINEAR(x4_lo, x4_hi, x1_lo, x1_hi,  7, 41 -  7, t0, t1);         \
            RV_LINEAR(x1_lo, x1_hi, x3_lo, x3_hi, 39, 61 - 39, t0, t1);         \
            RV_LINEAR(x3_lo, x3_hi, x3_tmp_lo, x3_tmp_hi, 10, 17 - 10, t0, t1); \
        } \
    }

#elif defined(__RV64__)

    #define RV64_PERM(n, x0, x1, x2, x3, x4, t0, t1, t2, x3_tmp) {  \
        for (int i = (12 - (n)); i < 12; i++)                       \
        {                                                           \
            x2 ^= ascon_round_constants[i];                         \
                                                                    \
            RV_S_BOX(x0, x1, x2, x3, x4, t0, t1, t2, x3_tmp);       \
                                                                    \
            RV_LINEAR(x0, x2, 19, 28, t0);                          \
            RV_LINEAR(x2, x4,  1,  6, t0);                          \
            RV_LINEAR(x4, x1,  7, 41, t0);                          \
            RV_LINEAR(x1, x3, 39, 61, t0);                          \
            RV_LINEAR(x3, x3_tmp, 10, 17, t0);                      \
        }                                                           \
    }

#else
    #error unimplemented
#endif