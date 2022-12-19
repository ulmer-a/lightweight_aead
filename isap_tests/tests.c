#include <_internal/testing.h>

int test_isap_a_128_encrypt();
int test_isap_a_128_decrypt();
int test_isap_a_128a_encrypt();
int test_isap_a_128a_decrypt();

void setup_tests()
{
    TEST(test_isap_a_128_encrypt, "isap_a_128/encrypt");
    TEST(test_isap_a_128_decrypt, "isap_a_128/decrypt");
    TEST(test_isap_a_128a_encrypt, "isap_a_128a/encrypt");
    TEST(test_isap_a_128a_decrypt, "isap_a_128a/decrypt");
}
