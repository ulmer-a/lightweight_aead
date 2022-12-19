#include <_internal/testing.h>

int test_load_n();
int test_store_n();
int test_load_64();
int test_store_64();
int u64big_test();
int interleave_cons_test();
int bi32_interleave_16_test();
int bi32_deinterleave_16_test();
int bi32_interleave_32_test();
int bi32_deinterleave_32_test();

void setup_tests()
{
    // (interleaved) load/store tests
    TEST(test_load_n, "word/load_n");
    TEST(test_store_n, "word/store_n");
    TEST(test_load_64, "word/load_64");
    TEST(test_store_64, "word/store_64");

    // General Bit-Interleaving tests
    TEST(u64big_test, "bi32/u64big");
    TEST(interleave_cons_test, "bi32/interleave_consistency");

    // Interleave Tests using ZBKB extension 
    TEST(bi32_interleave_16_test, "bi32/interleave16");
    TEST(bi32_deinterleave_16_test, "bi32/deinterleave16");
    TEST(bi32_interleave_32_test, "bi32/interleave32");
    TEST(bi32_deinterleave_32_test, "bi32/deinterleave32");
}