#include <_internal/common.h>
#include "isap.h"
#include <ascon/permutation.h>

#if defined(__BI32__)

    #include "interleaving.h"

    #if defined(__RV32__)
        #define PERM(n) RV32_PERM((n), x3_tmp_lo, x3_tmp_hi,    \
            x0_lo, x0_hi, x1_lo, x1_hi, x2_lo, x2_hi,           \
            x3_lo, x3_hi, x4_lo, x4_hi, t0, t1, t2)
        
        #define LOAD_SWAP_DEINTERLEAVE_32(lo, hi, ptr)          \
            RV32_LOAD_SWAP_DEINTERLEAVE_32(lo, hi, t0, ptr)
    #else
        #error non-riscv architectures unimplemented permutation

        #define LOAD_SWAP_DEINTERLEAVE_32                       \
            GENERIC_LOAD_SWAP_DEINTERLEAVE_32
    #endif

    void isap_rk(
        const uint8_t *k,
        const uint8_t *iv,
        const uint8_t *y,
        uint8_t *out,
        const uint8_t outlen
    ) {
        register uint32_t x0_lo, x0_hi, x1_lo, x1_hi,
            x2_lo, x2_hi, x3_lo, x3_hi, x4_lo, x4_hi;

        #if defined(__RV32__)
            // additional state variables required for optimized
            // implementation on rv32
            register uint32_t x3_tmp_lo, x3_tmp_hi, t0, t1, t2;
        #endif

        LOAD_SWAP_DEINTERLEAVE_32(x0_lo, x0_hi, k);
        LOAD_SWAP_DEINTERLEAVE_32(x1_lo, x1_hi, k + 8);
        LOAD_SWAP_DEINTERLEAVE_32(x2_lo, x2_hi, iv);
        x3_lo = x3_hi = x4_lo = x4_hi = 0;

        PERM(ISAP_sK);

        for (uint8_t i = 0; i < 127; i++)
        {
            uint8_t cur_byte_pos = i / 8;
            uint8_t cur_bit_pos = 7 - (i % 8);
            uint32_t cur_bit = ((y[cur_byte_pos] >> (cur_bit_pos)) & 0x01) << 7;
            x0_hi ^= ((uint32_t)cur_bit) << 24;

            PERM(ISAP_sB);
        }

        uint8_t cur_bit = ((y[15]) & 0x01) << 7;
        x0_hi ^= ((uint32_t)cur_bit) << (24);

        PERM(ISAP_sK);

        *(uint32_t*)(out + 0) = x0_hi;
        *(uint32_t*)(out + 4) = x0_lo;
        *(uint32_t*)(out + 8) = x1_hi;
        *(uint32_t*)(out + 12) = x1_lo;

        if (outlen > 16) {
            *(uint32_t*)(out + 16) = x2_hi;
            *(uint32_t*)(out + 20) = x2_lo;
        }
    }

#else

    #if defined(__RV64__)
        #define PERM(n) RV64_PERM(n, \
            s->x[0], s->x[1], s->x[2], s->x[3], s->x[4], t0, t1, t2, x3_tmp)
    #else
        #error non-riscv architectures unimplemented permutation
    #endif

    void isap_rk(
        const uint8_t *k,
        const uint8_t *iv,
        const uint8_t *y,
        uint8_t *out_ptr,
        const uint8_t outlen
    ) {
        state_t state;
        state_t *s = &state, *out = (state_t*)out_ptr;

        register uint64_t t0, t1, t2, x3_tmp;

        // Initialize
        s->x[0] = U64BIG(*(uint64_t*)(k + 0));
        s->x[1] = U64BIG(*(uint64_t*)(k + 8));
        s->x[2] = U64BIG(*(uint64_t*)(iv + 0));
        s->x[3] = 0;
        s->x[4] = 0;

        PERM(ISAP_sK);

        for (unsigned long i = 0; i < 16; i++)
        {
            uint8_t y_byte = *y;

            s->b[0][7] ^= (y_byte & 0x80) << 0;
            PERM(ISAP_sB);
            s->b[0][7] ^= (y_byte & 0x40) << 1;
            PERM(ISAP_sB);
            s->b[0][7] ^= (y_byte & 0x20) << 2;
            PERM(ISAP_sB);
            s->b[0][7] ^= (y_byte & 0x10) << 3;
            PERM(ISAP_sB);
            s->b[0][7] ^= (y_byte & 0x08) << 4;
            PERM(ISAP_sB);
            s->b[0][7] ^= (y_byte & 0x04) << 5;
            PERM(ISAP_sB);
            s->b[0][7] ^= (y_byte & 0x02) << 6;
            PERM(ISAP_sB);
            s->b[0][7] ^= (y_byte & 0x01) << 7;

            if (i != 15) {
                PERM(ISAP_sB);
                y += 1;
            }
        }

        PERM(ISAP_sK);

        out->x[0] = s->x[0];
        out->x[1] = s->x[1];

        if (outlen > 16) {
            out->x[2] = s->x[2];
        }
    }

#endif // !defined(__BI32__)
