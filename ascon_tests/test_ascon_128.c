#include <_internal/testing.h>
#include <ascon/ascon.h>

extern const testcase_t ascon_128_testcases[];
extern const unsigned long ascon_128_testcases_count;

int test_ascon_128_encrypt()
{
    for (int i = 0; i < ascon_128_testcases_count; i++)
    {
        const testcase_t* tc = &ascon_128_testcases[i];

        char tag_buffer[16];
        char* ct_buffer = alloca(tc->ct_size);

        ascon_128_encrypt(
            tc->pt, tc->pt_size,
            tc->ad, tc->ad_size,
            tc->key, tc->npub,
            tag_buffer, ct_buffer
        );

        assert(memcmp(tag_buffer, tc->tag, 16) == 0);
        assert(memcmp(ct_buffer, tc->ct, tc->ct_size) == 0);
    }

    return TEST_SUCCESS;
}

int test_ascon_128_decrypt()
{
    for (int i = 0; i < ascon_128_testcases_count; i++)
    {
        const testcase_t* tc = &ascon_128_testcases[i];

        char* pt_buffer = alloca(tc->ct_size);

        int ret = ascon_128_decrypt(
            tc->ct, tc->ct_size,
            tc->ad, tc->ad_size,
            tc->key, tc->npub,
            tc->tag, pt_buffer
        );

        assert(ret == 0);
        assert(memcmp(pt_buffer, tc->pt, tc->pt_size) == 0);
    }

    return TEST_SUCCESS;
}
