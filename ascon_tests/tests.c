#include <_internal/testing.h>

int test_ascon_128_encrypt();
int test_ascon_128_decrypt();
int test_ascon_128a_encrypt();
int test_ascon_128a_decrypt();

void setup_tests()
{
    TEST(test_ascon_128_encrypt, "ascon_128/encrypt");
    TEST(test_ascon_128_decrypt, "ascon_128/decrypt");
    TEST(test_ascon_128a_encrypt, "ascon_128a/encrypt");
    TEST(test_ascon_128a_decrypt, "ascon_128a/decrypt");
}
