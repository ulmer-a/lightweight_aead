#include <_internal/testing.h>

void bench_isap_a_128_encrypt(int);
void bench_isap_a_128_decrypt(int);
void bench_isap_a_128a_encrypt(int);
void bench_isap_a_128a_decrypt(int);

void setup_tests()
{
    BENCH(bench_isap_a_128_encrypt, "isap_a_128/encrypt");
    BENCH(bench_isap_a_128_decrypt, "isap_a_128/decrypt");
    BENCH(bench_isap_a_128a_encrypt, "isap_a_128a/encrypt");
    BENCH(bench_isap_a_128a_decrypt, "isap_a_128a/decrypt");
}
