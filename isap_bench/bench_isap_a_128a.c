#include <isap/isap.h>
#include <_internal/testing.h>

void bench_isap_a_128a_encrypt(int msg_len)
{
    char tag_buffer[16], ct_buffer[msg_len];
    isap_a_128a_encrypt(
        __RANDOM__, msg_len,
        __RANDOM__, 8,
        __RANDOM__ + 20,
        __RANDOM__ + 39,
        tag_buffer, ct_buffer
    );
}

void bench_isap_a_128a_decrypt(int msg_len)
{
    char ct_buffer[msg_len];
    isap_a_128a_decrypt(
        __RANDOM__, msg_len,
        __RANDOM__, 8,
        __RANDOM__ + 20,
        __RANDOM__ + 39,
        __RANDOM__ + 3, ct_buffer
    );
}