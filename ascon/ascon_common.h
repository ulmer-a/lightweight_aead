#pragma once

#include <_internal/common.h>
#include <ascon/permutation.h>
#include "ascon_common.h"
#include "word.h"

#define CRYPTO_ABYTES 16

#define ASCON_128_KEYBYTES 16
#define ASCON_128A_KEYBYTES 16
#define ASCON_80PQ_KEYBYTES 20

#if defined(__BI32__)
    #define ASCON_128_IV    0x8021000008220000ull
	#define ASCON_128A_IV 	0x8822000000200000ull
	#define ASCON_80PQ_IV 	0xc021000008220000ull
#else
    #define ASCON_128_IV    0x80400c0600000000ull
    #define ASCON_128A_IV   0x80800c0800000000ull
    #define ASCON_80PQ_IV   0xa0400c06ull
#endif

#if defined(ASCON_128)
	#define ascon_init 			ascon_128_init
	#define ascon_assoc_data 	ascon_128_assoc_data
	#define ascon_encrypt 		ascon_128_encrypt
	#define ascon_encrypt_impl	ascon_128_encrypt_impl
	#define ascon_decrypt 		ascon_128_decrypt
	#define ascon_decrypt_impl	ascon_128_decrypt_impl
	#define ascon_finalize 		ascon_128_finalize

	#define P_A					P12
	#define P_B					P6
	#define ASCON_AEAD_RATE 	8
	#define ASCON_KEYBYTES		16
	#define ASCON_PA_ROUNDS		12
	#define ASCON_PB_ROUNDS		6
	#define ASCON_IV			ASCON_128_IV
	#define CRYPTO_KEYBYTES		ASCON_128_KEYBYTES
#elif defined(ASCON_128A)
	#define ascon_init 			ascon_128a_init
	#define ascon_assoc_data 	ascon_128a_assoc_data
	#define ascon_encrypt 		ascon_128a_encrypt
	#define ascon_encrypt_impl	ascon_128a_encrypt_impl
	#define ascon_decrypt 		ascon_128a_decrypt
	#define ascon_decrypt_impl	ascon_128a_decrypt_impl
	#define ascon_finalize 		ascon_128a_finalize

	#define P_A					P12
	#define P_B					P8
	#define ASCON_AEAD_RATE 	16
	#define ASCON_KEYBYTES		16
	#define ASCON_PA_ROUNDS		12
	#define ASCON_PB_ROUNDS		8
	#define ASCON_IV			ASCON_128A_IV
	#define CRYPTO_KEYBYTES		ASCON_128A_KEYBYTES
#else
	#error neither ASCON_128 nor ASCON_128a is defined!
#endif

void ascon_init(state_t* state, const uint8_t* nonce,
    const uint64_t key0, const uint64_t key1);

void ascon_assoc_data(state_t* state,
    const uint8_t* assoc_data, uint64_t assoc_data_len);

void ascon_finalize(state_t* state, uint8_t* tag,
    const uint64_t key0, const uint64_t key1);
