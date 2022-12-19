#include <_internal/testing.h>

void bench_ascon_128_encrypt(int);
void bench_ascon_128_decrypt(int);
void bench_ascon_128a_encrypt(int);
void bench_ascon_128a_decrypt(int);

void setup_tests()
{
    BENCH(bench_ascon_128_encrypt, "ascon_128/encrypt");
    BENCH(bench_ascon_128_decrypt, "ascon_128/decrypt");
    BENCH(bench_ascon_128a_encrypt, "ascon_128a/encrypt");
    BENCH(bench_ascon_128a_decrypt, "ascon_128a/decrypt");
}
