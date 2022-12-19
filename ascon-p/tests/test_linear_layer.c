#include <_internal/testing.h>
#include <_internal/state.h>

#include "../linear_layer.h"

#if defined(__RV32__)

    int test_linear_layer()
    {
        state_t src1, src2, dst2;
        get_random_state(&src1, &src2);

        generic_linear_layer(&src1);

        size_t t0, t1;
        RV_LINEAR(dst2.w[0].l, dst2.w[0].h, src2.w[0].l, src2.w[0].h, 19, 28 - 19, t0, t1);
        RV_LINEAR(dst2.w[1].l, dst2.w[1].h, src2.w[1].l, src2.w[1].h, 39, 61 - 39, t0, t1);
        RV_LINEAR(dst2.w[2].l, dst2.w[2].h, src2.w[2].l, src2.w[2].h,  1,  6 -  1, t0, t1);
        RV_LINEAR(dst2.w[3].l, dst2.w[3].h, src2.w[3].l, src2.w[3].h, 10, 17 - 10, t0, t1);
        RV_LINEAR(dst2.w[4].l, dst2.w[4].h, src2.w[4].l, src2.w[4].h,  7, 41 -  7, t0, t1);

        assert(memcmp(&src1, &dst2, sizeof(state_t)) == 0);
        return TEST_SUCCESS;
    }

#elif defined(__RV64__)

    int test_ror64() { return TEST_SKIPPED; }

    int test_linear_layer()
    {
        size_t t0;
        state_t src1, src2, dst2;
        get_random_state(&src1, &src2);

        generic_linear_layer(&src1);

        RV_LINEAR(dst2.x[0], src2.x[0], 19, 28, t0);
        RV_LINEAR(dst2.x[1], src2.x[1], 39, 61, t0);
        RV_LINEAR(dst2.x[2], src2.x[2],  1,  6, t0);
        RV_LINEAR(dst2.x[3], src2.x[3], 10, 17, t0);
        RV_LINEAR(dst2.x[4], src2.x[4],  7, 41, t0);

        assert(memcmp(&src1, &dst2, sizeof(state_t)) == 0);
        return TEST_SUCCESS;
    }

#else

    #error linear layer tests unimplemented for other architecture than RISC-V

#endif
