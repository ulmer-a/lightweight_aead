#include <_internal/common.h>
#include "isap.h"
#include <ascon/permutation.h>

const static uint8_t ISAP_IV_KE[] = {
    0x03, ISAP_K, ISAP_rH, ISAP_rB, ISAP_sH, ISAP_sB, ISAP_sE, ISAP_sK
};

#if defined(__BI32__)

    #include "interleaving.h"

    #if defined(__RV32__)
        #define PERM(n) { \
            uint32_t t1, t2, t3, t4; \
            RV32_PERM((n), t1, t2,    \
            x0.e, x0.o, x1.e, x1.o, x2.e, x2.o,           \
            x3.e, x3.o, x4.e, x4.o, t0, t3, t4) \
        }
        
        #define LOAD_SWAP_DEINTERLEAVE_32(lo, hi, ptr)          \
            RV32_LOAD_SWAP_DEINTERLEAVE_32(lo, hi, t0, ptr)
        
        #define SWAP_INTERLEAVE_32(out, in) \
            RV32_SWAP_INTERLEAVE_32(out.e, out.o, in.e, in.o, t0)
    #else
        #error non-riscv architectures unimplemented permutation

        #define LOAD_SWAP_DEINTERLEAVE_32                       \
            GENERIC_LOAD_SWAP_DEINTERLEAVE_32
    #endif

    void isap_enc(
        const uint8_t *k,
        const uint8_t *npub,
        const uint8_t *m, uint64_t mlen,
        uint8_t *c)
    {
        // Derive Ke
        uint8_t ke[ISAP_STATE_SZ - CRYPTO_NPUBBYTES];
        isap_rk(k, ISAP_IV_KE, npub, ke, ISAP_STATE_SZ - CRYPTO_NPUBBYTES);

        // State and temporary variables
        u32_2 x0, x1, x2, x3, x4;
        uint32_t t0;

        // Init State
        x0.o = *(uint32_t *)(ke + 0);
        x0.e = *(uint32_t *)(ke + 4);
        x1.o = *(uint32_t *)(ke + 8);
        x1.e = *(uint32_t *)(ke + 12);
        x2.o = *(uint32_t *)(ke + 16);
        x2.e = *(uint32_t *)(ke + 20);
        LOAD_SWAP_DEINTERLEAVE_32(x3.e, x3.o, npub);
        LOAD_SWAP_DEINTERLEAVE_32(x4.e, x4.o, npub + 8);

        // Squeeze full lanes
        while (mlen >= 8)
        {
            PERM(ISAP_sE);

            u32_2 tmp;
            SWAP_INTERLEAVE_32(tmp, x0);
            *(uint64_t *)c = *(uint64_t *)m ^ *(uint64_t*)&tmp;

            mlen -= 8;
            m += ISAP_rH / 8;
            c += ISAP_rH / 8;
        }

        // Squeeze partial lane
        if (mlen > 0)
        {
            PERM(ISAP_sE);
            u32_2 tmp;
            SWAP_INTERLEAVE_32(tmp, x0);
            uint8_t *tmp0_bytes = (uint8_t *)&tmp;
            for (uint8_t i = 0; i < mlen; i++)
            {
                *c = *m ^ tmp0_bytes[i];
                m += 1;
                c += 1;
            }
        }
    }

#else

    #if defined(__RV64__)
        #define PERM(n) {                                                   \
            uint64_t t0, t1, t2, x3_tmp;                                    \
            RV64_PERM(n, s->x[0], s->x[1], s->x[2], s->x[3], s->x[4],       \
                t0, t1, t2, x3_tmp)                                         \
        }
    #endif

    void isap_enc(
	    const unsigned char *k,
	    const unsigned char *npub,
	    const unsigned char *m,
        unsigned long long mlen,
	    unsigned char *c
    ){
        state_t state;
        state_t *s = &state;

        isap_rk(k, ISAP_IV_KE, npub, (uint8_t*)s, ISAP_STATE_SZ - CRYPTO_NPUBBYTES);
        s->x[3] = U64BIG(*(uint64_t*)(npub + 0));
        s->x[4] = U64BIG(*(uint64_t*)(npub + 8));

        while (mlen >= ISAP_rH / 8)
        {
            PERM(ISAP_sE);

            *(uint64_t *)c = *(uint64_t *)m ^ U64BIG(s->x[0]);
            mlen -= ISAP_rH / 8;
            m += ISAP_rH / 8;
            c += ISAP_rH / 8;
        }

        if (mlen > 0)
        {
            PERM(ISAP_sE);

            lane_t t0;
            t0.x = U64BIG(s->x[0]);
            for (uint8_t i = 0; i < mlen; i++)
            {
                *c = *m ^ t0.b[i];
                m += 1;
                c += 1;
            }
        }
    }

#endif
