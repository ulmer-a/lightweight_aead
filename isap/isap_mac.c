#include <_internal/common.h>
#include "isap.h"
#include <ascon/permutation.h>

static const uint8_t ISAP_IV_A[]  = {
    0x01, ISAP_K, ISAP_rH, ISAP_rB, ISAP_sH, ISAP_sB, ISAP_sE, ISAP_sK
};

static const uint8_t ISAP_IV_KA[] = {
    0x02, ISAP_K, ISAP_rH, ISAP_rB, ISAP_sH, ISAP_sB, ISAP_sE, ISAP_sK
};

#if defined(__BI32__)

    #include "interleaving.h"

    #if defined(__RV32__)
        #define PERM(n) RV32_PERM((n), t0.e, t0.o,    \
            x0.e, x0.o, x1.e, x1.o, x2.e, x2.o,           \
            x3.e, x3.o, x4.e, x4.o, t1, t2, t3)
        
        #define LOAD_SWAP_DEINTERLEAVE_32(lo, hi, ptr)          \
            RV32_LOAD_SWAP_DEINTERLEAVE_32(lo, hi, t1, ptr)
        
        #define SWAP_INTERLEAVE_32(out, in) \
            RV32_SWAP_INTERLEAVE_32(out.e, out.o, in.e, in.o, t1)
    #else
        #error non-riscv architectures unimplemented permutation

        #define LOAD_SWAP_DEINTERLEAVE_32                       \
            GENERIC_LOAD_SWAP_DEINTERLEAVE_32
    #endif

    void isap_mac(
	    const unsigned char *k,
	    const unsigned char *npub,
	    const unsigned char *ad, unsigned long long adlen,
	    const unsigned char *c, unsigned long long clen,
	    unsigned char *tag
    ) {
        // State and temporary variables
        u32_2 x0, x1, x2, x3, x4;
        u32_2 t0;
        uint32_t t1, t2, t3;

        // Initialize
        LOAD_SWAP_DEINTERLEAVE_32(x0.e, x0.o, npub + 0);
        LOAD_SWAP_DEINTERLEAVE_32(x1.e, x1.o, npub + 8);
        LOAD_SWAP_DEINTERLEAVE_32(x2.e, x2.o, ISAP_IV_A);
        x3.o = 0;
        x3.e = 0;
        x4.o = 0;
        x4.e = 0;
        PERM(ISAP_sH);

        // Absorb full lanes of AD
        while (adlen >= 8)
        {
            LOAD_SWAP_DEINTERLEAVE_32(t0.e, t0.o, ad);
            x0.e ^= t0.e;
            x0.o ^= t0.o;
            adlen -= ISAP_rH / 8;
            ad += ISAP_rH / 8;
            PERM(ISAP_sH);
        }

        // Absorb partial lane of AD and add padding
        if (adlen > 0)
        {
            uint64_t tmp0 = 0;
            uint8_t *tmp0_bytes = (uint8_t *)&tmp0;
            int i;
            for (i = 0; i < adlen; i++)
            {
                tmp0_bytes[i] = *ad;
                ad += 1;
            }
            tmp0_bytes[i] = 0x80;
            LOAD_SWAP_DEINTERLEAVE_32(t0.e, t0.o, &tmp0);
            x0.e ^= t0.e;
            x0.o ^= t0.o;
            PERM(ISAP_sH);
        }

        // Absorb AD padding if not already done before
        if (adlen == 0)
        {
            x0.o ^= 0x80000000;
            PERM(ISAP_sH);
        }

        // Domain Seperation
        x4.e ^= ((uint32_t)0x01);

        // Absorb full lanes of C
        while (clen >= 8)
        {
            LOAD_SWAP_DEINTERLEAVE_32(t0.e, t0.o, c);
            x0.e ^= t0.e;
            x0.o ^= t0.o;
            PERM(ISAP_sH);
            clen -= ISAP_rH / 8;
            c += ISAP_rH / 8;
        }

        // Absorb partial lane of C and add padding
        if (clen > 0)
        {
            uint64_t tmp0 = 0;
            uint8_t *tmp0_bytes = (uint8_t *)&tmp0;
            int i;
            for (i = 0; i < clen; i++)
            {
                tmp0_bytes[i] = *c;
                c += 1;
            }
            tmp0_bytes[i] = 0x80;
            LOAD_SWAP_DEINTERLEAVE_32(t0.e, t0.o, &tmp0);
            x0.e ^= t0.e;
            x0.o ^= t0.o;
            PERM(ISAP_sH);
        }

        // Absorb C padding if not already done before
        if (clen == 0)
        {
            x0.o ^= 0x80000000;
            PERM(ISAP_sH);
        }

        // Finalize - Derive Ka*
        u32_2 tmp;
        uint64_t y64[CRYPTO_KEYBYTES / 8];
        SWAP_INTERLEAVE_32(tmp, x0);
        y64[0] = *(uint64_t*)&tmp;
        SWAP_INTERLEAVE_32(tmp, x1);
        y64[1] = *(uint64_t*)&tmp;

        uint32_t ka_star32[CRYPTO_KEYBYTES / 4];
        isap_rk(k, ISAP_IV_KA, (uint8_t *)y64, (uint8_t *)ka_star32, CRYPTO_KEYBYTES);

        // Finalize - Squeeze T
        x0.o = ka_star32[0];
        x0.e = ka_star32[1];
        x1.o = ka_star32[2];
        x1.e = ka_star32[3];
        PERM(ISAP_sH);
        SWAP_INTERLEAVE_32(tmp, x0);
        *(uint64_t *)(tag + 0) = *(uint64_t*)&tmp;
        SWAP_INTERLEAVE_32(tmp, x1);
        *(uint64_t *)(tag + 8) = *(uint64_t*)&tmp;
    }

#else

    #if defined(__RV64__)
        #define PERM(n) {                                                   \
            uint64_t t0, t1, t2, x3_tmp;                                    \
            RV64_PERM(n, s->x[0], s->x[1], s->x[2], s->x[3], s->x[4],       \
                t0, t1, t2, x3_tmp)                                         \
        }
    #endif

    static inline void ABSORB_LANES(state_t *s, const uint8_t *src, uint64_t len)
    {
        while (len >= 8)
        {
            s->x[0] ^= U64BIG(*(uint64_t*)src);
            len -= ISAP_rH / 8;
            src += ISAP_rH / 8;

            PERM(ISAP_sH);
        }

        if (len > 0)
        {
            unsigned long i;
            lane_t t0 = {0};
            for (i = 0; i < len; i++)
            {
                t0.b[7 - i] ^= *src;
                src++;
            }
            t0.b[7 - i] ^= 0x80;
            //t0 = TOBI(t0);
            s->x[0] ^= t0.x;

            PERM(ISAP_sH);
        }
        else
        {
            // Absorb padded empty lane
            s->b[0][7] ^= 0x80;

            PERM(ISAP_sH);
        }
    }

    void isap_mac(
	    const unsigned char *k,
	    const unsigned char *npub,
	    const unsigned char *ad, unsigned long long adlen,
	    const unsigned char *c, unsigned long long clen,
	    unsigned char *tag
    ) {
        state_t state;
        state_t *s = &state;

        s->x[0] = U64BIG(*(uint64_t *)(npub + 0));
        s->x[1] = U64BIG(*(uint64_t *)(npub + 8));
        s->x[2] = U64BIG(*(uint64_t *)(ISAP_IV_A + 0));
        s->x[3] = 0;
        s->x[4] = 0;

        PERM(ISAP_sH);

        ABSORB_LANES(s, ad, adlen);

        s->w32[4][0] ^= 0x1UL;

        ABSORB_LANES(s, c, clen);

        s->x[0] = U64BIG(s->x[0]);
        s->x[1] = U64BIG(s->x[1]);
        isap_rk(k, ISAP_IV_KA, (const uint8_t *)(s->b), (unsigned char*)s, CRYPTO_KEYBYTES);

        PERM(ISAP_sH);

        uint64_t t0 = U64BIG(s->x[0]);
        memcpy(tag + 0, &t0, 8);
        t0 = U64BIG(s->x[1]);
        memcpy(tag + 8, &t0, 8);
    }

#endif
