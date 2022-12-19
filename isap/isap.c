#include "isap.h"

#if defined(ISAP_128)
	#define isap_a_encrypt isap_a_128_encrypt
	#define isap_a_decrypt isap_a_128_decrypt
#elif defined(ISAP_128A)
	#define isap_a_encrypt isap_a_128a_encrypt
	#define isap_a_decrypt isap_a_128a_decrypt
#else
	#error neither ISAP_128 nor ISAP_128a is defined!
#endif

int isap_a_encrypt(
	const unsigned char *message, unsigned long long message_len,
	const unsigned char *assoc_data, unsigned long long assoc_data_len,
	const unsigned char *key, const unsigned char *nonce,
	unsigned char *tag_out, unsigned char *ciphertext_out
) {
	if (message_len > 0) {
		isap_enc(key, nonce, message, message_len, ciphertext_out);
	}

	isap_mac(key, nonce, assoc_data, assoc_data_len,
		ciphertext_out, message_len, tag_out);

	return 0;
}

int isap_a_decrypt(
	const unsigned char *ciphertext, unsigned long long ciphertext_len,
	const unsigned char *assoc_data, unsigned long long assoc_data_len,
	const unsigned char *key, const unsigned char* nonce,
	const unsigned char *tag, unsigned char *message_out
) {
	unsigned char actual_tag[ISAP_TAG_SZ];
	isap_mac(key, nonce, assoc_data, assoc_data_len,
		ciphertext, ciphertext_len, actual_tag);

	// constant-time tag comparison
	unsigned long eq_cnt = 0;
	for (unsigned int i = 0; i < ISAP_TAG_SZ; i++) {
		eq_cnt += (actual_tag[i] == tag[i]);
	}

	int success = (eq_cnt == (unsigned long)ISAP_TAG_SZ) ? 0 : -1;

	// only decrypt, if tag was correct but always decrypt when
	// testing is enabled (for correct benchmarks)
	if (TESTING_ENABLED || success == 0) {
		if (ciphertext_len > 0) {
			isap_enc(key, nonce, ciphertext, ciphertext_len, message_out);
		}
	}

	return success;
}
