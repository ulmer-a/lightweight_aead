#include "ascon_common.h"

#if !defined(ascon_init) || !defined(ascon_assoc_data) || !defined(ascon_finalize)
    #warning please include ascon_common.h
#endif

void ascon_init(state_t* state, const uint8_t* nonce,
    const uint64_t key0, const uint64_t key1)
{
    const uint64_t N0 = LOAD_64(nonce);
    const uint64_t N1 = LOAD_64(nonce + 8);

    state->x[0] = ASCON_IV;
    state->x[1] = key0;
    state->x[2] = key1;
    state->x[3] = N0;
    state->x[4] = N1;

    P_A(state);

    state->x[3] ^= key0;
    state->x[4] ^= key1;
}

void ascon_assoc_data(state_t* state,
    const uint8_t* assoc_data, uint64_t assoc_data_len)
{
    if (assoc_data_len)
    {
        while (assoc_data_len >= ASCON_AEAD_RATE)
        {
            state->x[0] ^= LOAD_64(assoc_data);
            if (ASCON_AEAD_RATE == 16) {
                state->x[1] ^= LOAD_64(assoc_data + 8);
            }

            P_B(state);

            assoc_data += ASCON_AEAD_RATE;
            assoc_data_len -= ASCON_AEAD_RATE;
        }

        uint64_t* px = &state->x[0];
        if (ASCON_AEAD_RATE == 16 && assoc_data_len >= 8)
        {
            state->x[0] ^= LOAD_64(assoc_data);
            px = &state->x[1];
            assoc_data += 8;
            assoc_data_len -= 8;
        }

        *px ^= PAD(assoc_data_len);
        if (assoc_data_len) {
            *px ^= LOAD_N(assoc_data, assoc_data_len);
        }

        P_B(state);
    }

    state->x[4] ^= 1;
}

void ascon_finalize(state_t* state, uint8_t* tag,
    const uint64_t key0, const uint64_t key1)
{
    #if ASCON_AEAD_RATE == 8
        state->x[1] ^= key0;
        state->x[2] ^= key1;
    #else
        state->x[2] ^= key0;
        state->x[3] ^= key1;
    #endif

    P_A(state);

    state->x[3] ^= key0;
    state->x[4] ^= key1;

    STORE_64(tag, state->x[3]);
    STORE_64(tag + 8, state->x[4]);
}
