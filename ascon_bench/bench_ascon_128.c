#include <ascon/ascon.h>
#include <_internal/testing.h>

void bench_ascon_128_encrypt(int msg_len)
{
    char tag_buffer[16], ct_buffer[msg_len];
    ascon_128_encrypt(
        __RANDOM__, msg_len,
        __RANDOM__, 8,
        __RANDOM__ + 20,
        __RANDOM__ + 39,
        tag_buffer, ct_buffer
    );
}

void bench_ascon_128_decrypt(int msg_len)
{
    char ct_buffer[msg_len];
    ascon_128_decrypt(
        __RANDOM__, msg_len,
        __RANDOM__, 8,
        __RANDOM__ + 20,
        __RANDOM__ + 39,
        __RANDOM__ + 3, ct_buffer
    );
}
