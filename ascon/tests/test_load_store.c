#include <_internal/testing.h>
#include <_internal/common.h>

#include "../word.h"

static uint64_t MASK(int n) {
  /* undefined for n == 0 */
  return ~0ull >> (64 - 8 * n);
}

#if defined(__BI32__)
    #define WORDTOU64 interleave32
    #define U64TOWORD deinterleave32
#else
    // TODO correct endianness
    #define WORDTOU64 __builtin_bswap64
    #define U64TOWORD __builtin_bswap64
#endif

static uint64_t load_n_ref(const uint8_t* bytes, int n) {
  uint64_t x = *(const uint64_t*)bytes & MASK(n);
  return U64TOWORD(x);
}

static void store_n_ref(uint8_t* bytes, uint64_t w, int n) {
  *(uint64_t*)bytes &= ~MASK(n);
  *(uint64_t*)bytes |= WORDTOU64(w);
}

const uint8_t* random_data = (void*)&load_n_ref;

int test_load_n()
{
    assert(load_n_ref(random_data, 8) == LOAD_N(random_data, 8));
    assert(load_n_ref(random_data, 7) == LOAD_N(random_data, 7));
    assert(load_n_ref(random_data, 3) == LOAD_N(random_data, 3));
    assert(load_n_ref(random_data, 1) == LOAD_N(random_data, 1));
    assert(load_n_ref(random_data, 4) == LOAD_N(random_data, 4));

    return TEST_SUCCESS;
}

int test_store_n()
{
    char store_buffer1[8];
    char store_buffer2[8];

    store_n_ref(store_buffer1, 0xdeadbeefbadeaffe, 3);
    STORE_N(store_buffer2, 0xdeadbeefbadeaffe, 3);
    assert(memcmp(store_buffer1, store_buffer2, 3) == 0);

    store_n_ref(store_buffer1, 0xdeadbadeaffebeef, 1);
    STORE_N(store_buffer2, 0xdeadbadeaffebeef, 1);
    assert(memcmp(store_buffer1, store_buffer2, 1) == 0);

    store_n_ref(store_buffer1, 0xbadeaffedeadbeef, 5);
    STORE_N(store_buffer2, 0xbadeaffedeadbeef, 5);
    assert(memcmp(store_buffer1, store_buffer2, 5) == 0);

    store_n_ref(store_buffer1, 0xbadeaffedeadbeef, 4);
    STORE_N(store_buffer2, 0xbadeaffedeadbeef, 4);
    assert(memcmp(store_buffer1, store_buffer2, 4) == 0);

    return TEST_SUCCESS;
}

int test_load_64()
{
    assert(load_n_ref(random_data, 8) == LOAD_64(random_data));
    assert(load_n_ref(random_data + 4, 8) == LOAD_64(random_data + 4));
    assert(load_n_ref(random_data + 1, 8) == LOAD_64(random_data + 1));
    assert(load_n_ref(random_data + 7, 8) == LOAD_64(random_data + 7));

    return TEST_SUCCESS;
}

int test_store_64()
{
    char store_buffer1[8];
    char store_buffer2[8];

    store_n_ref(store_buffer1, 0xdeadbeefbadeaffe, 8);
    STORE_64(store_buffer2, 0xdeadbeefbadeaffe);
    assert(memcmp(store_buffer1, store_buffer2, 8) == 0);

    store_n_ref(store_buffer1, 0xdeadbadeaffebeef, 8);
    STORE_64(store_buffer2, 0xdeadbadeaffebeef);
    assert(memcmp(store_buffer1, store_buffer2, 8) == 0);

    store_n_ref(store_buffer1, 0xbadeaffedeadbeef, 8);
    STORE_64(store_buffer2, 0xbadeaffedeadbeef);
    assert(memcmp(store_buffer1, store_buffer2, 8) == 0);
    
    return TEST_SUCCESS;
}
