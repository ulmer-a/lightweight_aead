#include "ascon_common.h"

#if !defined(ascon_encrypt)
    #warning please include ascon_common.h
#endif

void ascon_encrypt_impl(state_t* state, uint8_t* ciphertext,
    const uint8_t* message, uint64_t message_len)
{
    // full plaintext blocks
    while (message_len >= ASCON_AEAD_RATE)
    {
        state->x[0] ^= LOAD_N(message, 8);
        STORE_N(ciphertext, state->x[0], 8);

        if (ASCON_AEAD_RATE == 16)
        {
            state->x[1] ^= LOAD_N(message + 8, 8);
            STORE_N(ciphertext + 8, state->x[1], 8);
        }

        P_B(state);

        message     += ASCON_AEAD_RATE;
        ciphertext  += ASCON_AEAD_RATE;
        message_len -= ASCON_AEAD_RATE;
    }

    // final plaintext block
    uint64_t* current_x = &state->x[0];
    if (ASCON_AEAD_RATE == 16 && message_len >= 8)
    {
        state->x[0] ^= LOAD_N(message, 8);
        STORE_N(ciphertext, state->x[0], 8);
        current_x = &state->x[1];

        message     += 8;
        ciphertext  += 8;
        message_len -= 8;
    }

    *current_x ^= PAD(message_len);
    if (message_len > 0) {
        *current_x ^= LOAD_N(message, message_len);
        STORE_N(ciphertext, *current_x, message_len);
    }
}

int ascon_encrypt(
    const unsigned char *message, unsigned long long message_len,
    const unsigned char *assoc_data, unsigned long long assoc_data_len,
    const unsigned char *key, const unsigned char *nonce,
    unsigned char *tag_out, unsigned char *ciphertext_out
) {
    const uint64_t key0 = LOAD_64(key);
    const uint64_t key1 = LOAD_64(key + 8);

    state_t state;
    ascon_init(&state, nonce, key0, key1);
    ascon_assoc_data(&state, assoc_data, assoc_data_len);
    ascon_encrypt_impl(&state, ciphertext_out, message, message_len);
    ascon_finalize(&state, tag_out, key0, key1);
    return 0;
}
