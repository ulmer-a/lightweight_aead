#include <ascon/permutation.h>
#include <_internal/common.h>

#include "s_box.h"
#include "linear_layer.h"

#if !defined(__RV32__)

    #if defined(__RV64__)
        static forceinline void ROUND(state_t* s, uint64_t C)
        {
            register uint64_t x0, x1, x2, x3, x4;
            register uint64_t t0, t1, t2, x3_tmp;

            x0 = s->x[0];
            x1 = s->x[1];
            x2 = s->x[2];
            x3 = s->x[3];
            x4 = s->x[4];

            x2 ^= C;

            RV_S_BOX(x0, x1, x2, x3, x4, t0, t1, t2, x3_tmp);

            RV_LINEAR(x0, x2, 19, 28, t0);
            RV_LINEAR(x2, x4,  1,  6, t0);
            RV_LINEAR(x4, x1,  7, 41, t0);
            RV_LINEAR(x1, x3, 39, 61, t0);
            RV_LINEAR(x3, x3_tmp, 10, 17, t0);

            s->x[0] = x0;
            s->x[1] = x1;
            s->x[2] = x2;
            s->x[3] = x3;
            s->x[4] = x4;
        }
    #else
        static forceinline void ROUND(state_t* s, uint8_t C) {
            state_t t;

            // bitwise addition of round constant
            s->x[2] ^= C;
            
            // substitution layer
            s->x[0] ^= s->x[4];
            s->x[4] ^= s->x[3];
            s->x[2] ^= s->x[1];
            // start of keccak s-box
            t.x[0] = s->x[0] ^ (~s->x[1] & s->x[2]);
            t.x[1] = s->x[1] ^ (~s->x[2] & s->x[3]);
            t.x[2] = s->x[2] ^ (~s->x[3] & s->x[4]);
            t.x[3] = s->x[3] ^ (~s->x[4] & s->x[0]);
            t.x[4] = s->x[4] ^ (~s->x[0] & s->x[1]);
            // end of keccak s-box
            t.x[1] ^= t.x[0];
            t.x[0] ^= t.x[4];
            t.x[3] ^= t.x[2];
            t.x[2] = ~t.x[2];
            
            /* linear diffusion layer */
            s->x[0] = t.x[0] ^ ror64(t.x[0], 19) ^ ror64(t.x[0], 28);
            s->x[1] = t.x[1] ^ ror64(t.x[1], 61) ^ ror64(t.x[1], 39);
            s->x[2] = t.x[2] ^ ror64(t.x[2], 1)  ^ ror64(t.x[2], 6);
            s->x[3] = t.x[3] ^ ror64(t.x[3], 10) ^ ror64(t.x[3], 17);
            s->x[4] = t.x[4] ^ ror64(t.x[4], 7)  ^ ror64(t.x[4], 41);
        }
    #endif

    void P6(state_t* state)
    {
        ROUND(state, ascon_round_constants[6]);
        ROUND(state, ascon_round_constants[7]);
        ROUND(state, ascon_round_constants[8]);
        ROUND(state, ascon_round_constants[9]);
        ROUND(state, ascon_round_constants[10]);
        ROUND(state, ascon_round_constants[11]);
    }

    void P8(state_t* state)
    {
        ROUND(state, ascon_round_constants[4]);
        ROUND(state, ascon_round_constants[5]);
        ROUND(state, ascon_round_constants[6]);
        ROUND(state, ascon_round_constants[7]);
        ROUND(state, ascon_round_constants[8]);
        ROUND(state, ascon_round_constants[9]);
        ROUND(state, ascon_round_constants[10]);
        ROUND(state, ascon_round_constants[11]);
    }

    void P12(state_t* state)
    {
        ROUND(state, ascon_round_constants[0]);
        ROUND(state, ascon_round_constants[1]);
        ROUND(state, ascon_round_constants[2]);
        ROUND(state, ascon_round_constants[3]);
        ROUND(state, ascon_round_constants[4]);
        ROUND(state, ascon_round_constants[5]);
        ROUND(state, ascon_round_constants[6]);
        ROUND(state, ascon_round_constants[7]);
        ROUND(state, ascon_round_constants[8]);
        ROUND(state, ascon_round_constants[9]);
        ROUND(state, ascon_round_constants[10]);
        ROUND(state, ascon_round_constants[11]);
    }

#endif
