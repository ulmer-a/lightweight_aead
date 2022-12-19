#include "ascon_common.h"

#if !defined(ascon_decrypt)
    #warning please include ascon_common.h
#endif

void ascon_decrypt_impl(state_t* state, uint8_t* message,
    const uint8_t* ciphertext, uint64_t ciphertext_len)
{
  // full ciphertext blocks
  while (ciphertext_len >= ASCON_AEAD_RATE) {
    uint64_t cx = LOAD_64(ciphertext);
    state->x[0] ^= cx;
    STORE_64(message, state->x[0]);
    state->x[0] = cx;
    if (ASCON_AEAD_RATE == 16) {
      cx = LOAD_64(ciphertext + 8);
      state->x[1] ^= cx;
      STORE_64(message + 8, state->x[1]);
      state->x[1] = cx;
    }
    P_B(state);
    message += ASCON_AEAD_RATE;
    ciphertext += ASCON_AEAD_RATE;
    ciphertext_len -= ASCON_AEAD_RATE;
  }

  // final ciphertext block
  uint64_t* px = &state->x[0];
  if (ASCON_AEAD_RATE == 16 && ciphertext_len >= 8) {
    uint64_t cx = LOAD_64(ciphertext);
    state->x[0] ^= cx;
    STORE_64(message, state->x[0]);
    state->x[0] = cx;
    px = &state->x[1];
    message += 8;
    ciphertext += 8;
    ciphertext_len -= 8;
  }

  *px ^= PAD(ciphertext_len);
  if (ciphertext_len) {
    uint64_t cx = LOAD_N(ciphertext, ciphertext_len);
    *px ^= cx;
    STORE_N(message, *px, ciphertext_len);
    *px = CLEAR_N(*px, ciphertext_len);
    *px ^= cx;
  }
}

int ascon_decrypt(
    const unsigned char *ciphertext, unsigned long long ciphertext_len,
    const unsigned char *assoc_data, unsigned long long assoc_data_len,
    const unsigned char *key, const unsigned char* nonce,
    const unsigned char *tag, unsigned char *message_out
) {
  const uint64_t key0 = LOAD_N(key, 8);
  const uint64_t key1 = LOAD_N(key + 8, 8);

  state_t state;
  uint8_t t[16];

  ascon_init(&state, nonce, key0, key1);
  ascon_assoc_data(&state, assoc_data, assoc_data_len);
  ascon_decrypt_impl(&state, message_out, ciphertext, ciphertext_len);
  ascon_finalize(&state, t, key0, key1);

  /* verify tag (should be constant time, check compiler output) */
  int result = 0;
  for (int i = 0; i < CRYPTO_ABYTES; ++i)
      result |= tag[i] ^ t[i];
  result = (((result - 1) >> 8) & 1) - 1;

  return result;
}
