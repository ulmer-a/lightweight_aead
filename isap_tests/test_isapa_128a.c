#include <_internal/testing.h>
#include <isap/isap.h>

extern const testcase_t isap_a_128a_testcases[];
extern const unsigned long isap_a_128a_testcases_count;

int test_isap_a_128a_encrypt()
{
    for (int i = 0; i < isap_a_128a_testcases_count; i++)
    {
        const testcase_t* tc = &isap_a_128a_testcases[i];

        char tag_buffer[16];
        char* ct_buffer = alloca(tc->ct_size);

        isap_a_128a_encrypt(
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

int test_isap_a_128a_decrypt()
{
    for (int i = 0; i < isap_a_128a_testcases_count; i++)
    {
        const testcase_t* tc = &isap_a_128a_testcases[i];

        char* pt_buffer = alloca(tc->ct_size);

        int ret = isap_a_128a_decrypt(
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
