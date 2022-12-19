#pragma once

int ascon_128_encrypt(
    const unsigned char *message, unsigned long long message_len,
    const unsigned char *assoc_data, unsigned long long assoc_data_len,
    const unsigned char *key, const unsigned char *nonce,
    unsigned char *tag_out, unsigned char *ciphertext_out
);

int ascon_128_decrypt(
    const unsigned char *ciphertext, unsigned long long ciphertext_len,
    const unsigned char *assoc_data, unsigned long long assoc_data_len,
    const unsigned char *key, const unsigned char* nonce,
    const unsigned char *tag, unsigned char *message_out
);

int ascon_128a_encrypt(
    const unsigned char *message, unsigned long long message_len,
    const unsigned char *assoc_data, unsigned long long assoc_data_len,
    const unsigned char *key, const unsigned char *nonce,
    unsigned char *tag_out, unsigned char *ciphertext_out
);

int ascon_128a_decrypt(
    const unsigned char *ciphertext, unsigned long long ciphertext_len,
    const unsigned char *assoc_data, unsigned long long assoc_data_len,
    const unsigned char *key, const unsigned char* nonce,
    const unsigned char *tag, unsigned char *message_out
);
