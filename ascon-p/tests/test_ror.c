#include <_internal/testing.h>
#include <_internal/state.h>

#include "../linear_layer.h"

#if defined(__RV32__)

    int test_generic_ror()
    {
        assert(generic_ror32(0xdeadbeef,  4) == 0xfdeadbee);
        return TEST_SUCCESS;
    }

#elif defined(__RV64__)

    int test_generic_ror()
    {
        assert(generic_ror64(0xdeadbadeaffebeef, 24) == 0xfebeefdeadbadeaf);
        return TEST_SUCCESS;
    }

#else

    #error add architecture above

#endif

#if defined(__RV32__) && defined(__BI32__)

    int test_bi_ror64()
    {
        const uint64_t src = 0xdeadbadeaffebeef;
        my_word_t src_w = *(my_word_t*)&src;

        const uint64_t g_dst1 = generic_ror64(src, 14);
        const uint64_t g_dst2 = generic_ror64(src, 27);
        const uint64_t g_dst3 = generic_ror64(src, 61 - 39);

        my_word_t dst1, dst2, dst3;
        RV_ROR64(dst1.l, dst1.h, src_w.l, src_w.h, 14);
        RV_ROR64(dst2.l, dst2.h, src_w.l, src_w.h, 27);
        RV_ROR64(dst3.l, dst3.h, src_w.l, src_w.h, 61 - 39);

        assert(g_dst1 == *(uint64_t*)&dst1);
        assert(g_dst2 == *(uint64_t*)&dst2);
        assert(g_dst3 == *(uint64_t*)&dst3);

        return TEST_SUCCESS;
    }

#else

    int test_bi_ror64() { return TEST_SKIPPED; }

#endif 
