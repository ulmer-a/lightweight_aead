#include <_internal/testing.h>
#include <_internal/state.h>

#include "../s_box.h"

int test_s_box()
{
    state_t t, s1, s2;
    get_random_state(&s1, &s2);

    // printf("x0: %llx, x1: %llx, x2: %llx, x3: %llx, x4: %llx\n",
    //     s1.x[0], s1.x[1], s1.x[2], s1.x[3], s1.x[4]);
    // printf("x0: %llx, x1: %llx, x2: %llx, x3: %llx, x4: %llx\n",
    //     s2.x[0], s2.x[1], s2.x[2], s2.x[3], s2.x[4]);
    
    register uint32_t t0, t1, t2, x3_lo, x3_hi;
    RV_S_BOX(s1.w[0].l, s1.w[1].l, s1.w[2].l, s1.w[3].l, s1.w[4].l, t0, t1, t2, x3_lo);
    RV_S_BOX(s1.w[0].h, s1.w[1].h, s1.w[2].h, s1.w[3].h, s1.w[4].h, t0, t1, t2, x3_hi);
    s1.x[0] = s1.x[2];
    s1.x[2] = ~s1.x[4];
    s1.x[4] = s1.x[1];
    s1.x[1] = s1.x[3];    
    s1.w[3].l = x3_lo;
    s1.w[3].h = x3_hi;

    s2.x[0] ^= s2.x[4];
    s2.x[4] ^= s2.x[3];
    s2.x[2] ^= s2.x[1];
    t.x[0] = s2.x[0] ^ (~s2.x[1] & s2.x[2]);
    t.x[2] = s2.x[2] ^ (~s2.x[3] & s2.x[4]);
    t.x[4] = s2.x[4] ^ (~s2.x[0] & s2.x[1]);
    t.x[1] = s2.x[1] ^ (~s2.x[2] & s2.x[3]);
    t.x[3] = s2.x[3] ^ (~s2.x[4] & s2.x[0]);
    t.x[1] ^= t.x[0];
    t.x[3] ^= t.x[2];
    t.x[0] ^= t.x[4];

    // printf("x0: %llx, x1: %llx, x2: %llx, x3: %llx, x4: %llx\n",
    //     s1.x[0], s1.x[1], s1.x[2], s1.x[3], s1.x[4]);
    // printf("x0: %llx, x1: %llx, x2: %llx, x3: %llx, x4: %llx\n",
    //     t.x[0], t.x[1], t.x[2], t.x[3], t.x[4]);

    assert(memcmp(&s1, &t, sizeof(state_t)) == 0);
    return TEST_SUCCESS;
}
