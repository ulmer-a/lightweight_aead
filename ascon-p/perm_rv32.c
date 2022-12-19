#include <_internal/common.h>
#include <ascon/permutation.h>

#if defined(__RV32__)

    static forceinline void Pn(state_t* s, int n)
    {
        register uint32_t x0_lo, x0_hi;
        register uint32_t x1_lo, x1_hi;
        register uint32_t x2_lo, x2_hi;
        register uint32_t x3_lo, x3_hi;
        register uint32_t x4_lo, x4_hi;
        register uint32_t t0, t1, t2, x3_tmp_lo, x3_tmp_hi;

        // load the state from memory into registers. only the above
        // register variables are used, there are no more temp-variables.
        x0_lo = *(((uint32_t*)s) + 0); x0_hi = *(((uint32_t*)s) + 1);
        x1_lo = *(((uint32_t*)s) + 2); x1_hi = *(((uint32_t*)s) + 3);
        x2_lo = *(((uint32_t*)s) + 4); x2_hi = *(((uint32_t*)s) + 5);
        x3_lo = *(((uint32_t*)s) + 6); x3_hi = *(((uint32_t*)s) + 7);
        x4_lo = *(((uint32_t*)s) + 8); x4_hi = *(((uint32_t*)s) + 9);

        for (int i = (12 - n) * 2; i < 24; i += 2)
        {
            // add the round constant
            x2_lo ^= ascon_round_constants_bi[i];
            x2_hi ^= ascon_round_constants_bi[i + 1];

            // apply s-box
            RV_S_BOX(x0_lo, x1_lo, x2_lo, x3_lo, x4_lo, t0, t1, t2, x3_tmp_lo);
            RV_S_BOX(x0_hi, x1_hi, x2_hi, x3_hi, x4_hi, t0, t1, t2, x3_tmp_hi);

            // apply linear layer
            RV_LINEAR(x0_lo, x0_hi, x2_lo, x2_hi, 19, 28 - 19, t0, t1);
            RV_LINEAR(x2_lo, x2_hi, x4_lo, x4_hi,  1,  6 -  1, t0, t1);
            RV_LINEAR(x4_lo, x4_hi, x1_lo, x1_hi,  7, 41 -  7, t0, t1);
            RV_LINEAR(x1_lo, x1_hi, x3_lo, x3_hi, 39, 61 - 39, t0, t1);
            RV_LINEAR(x3_lo, x3_hi, x3_tmp_lo, x3_tmp_hi, 10, 17 - 10, t0, t1);
        }

        // write the state back to memory
        *(((uint32_t*)s) + 0) = x0_lo; *(((uint32_t*)s) + 1) = x0_hi; 
        *(((uint32_t*)s) + 2) = x1_lo; *(((uint32_t*)s) + 3) = x1_hi; 
        *(((uint32_t*)s) + 4) = x2_lo; *(((uint32_t*)s) + 5) = x2_hi; 
        *(((uint32_t*)s) + 6) = x3_lo; *(((uint32_t*)s) + 7) = x3_hi; 
        *(((uint32_t*)s) + 8) = x4_lo; *(((uint32_t*)s) + 9) = x4_hi;
    }

    void P6(state_t* state)
    {
        Pn(state, 6);
    }

    void P8(state_t* state)
    {
        Pn(state, 8);
    }

    void P12(state_t* state)
    {
        Pn(state, 12);
    }

#endif
