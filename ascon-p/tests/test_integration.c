#include <_internal/testing.h>
#include <ascon/permutation.h>
#include "../linear_layer.h"

#define ror64 generic_ror64

static void round_ref(state_t* s, uint64_t C)
{
    state_t t;

    // bitwise addition of round constant
    s->x[2] ^= C;
    
    // substitution layer
    s->x[0] ^= s->x[4];
    s->x[4] ^= s->x[3];
    s->x[2] ^= s->x[1];
    // start of keccak s-box
    t.x[0] = s->x[0] ^ (~s->x[1] & s->x[2]);
    t.x[1] = s->x[1] ^ (~s->x[2] & s->x[3]);
    t.x[2] = s->x[2] ^ (~s->x[3] & s->x[4]);
    t.x[3] = s->x[3] ^ (~s->x[4] & s->x[0]);
    t.x[4] = s->x[4] ^ (~s->x[0] & s->x[1]);
    // end of keccak s-box
    t.x[1] ^= t.x[0];
    t.x[0] ^= t.x[4];
    t.x[3] ^= t.x[2];
    t.x[2] = ~t.x[2];
    
    /* linear diffusion layer */
    s->x[0] = t.x[0] ^ ror64(t.x[0], 19) ^ ror64(t.x[0], 28);
    s->x[1] = t.x[1] ^ ror64(t.x[1], 61) ^ ror64(t.x[1], 39);
    s->x[2] = t.x[2] ^ ror64(t.x[2], 1)  ^ ror64(t.x[2], 6);
    s->x[3] = t.x[3] ^ ror64(t.x[3], 10) ^ ror64(t.x[3], 17);
    s->x[4] = t.x[4] ^ ror64(t.x[4], 7)  ^ ror64(t.x[4], 41);
}

static void ref_permutation(state_t* s, int n)
{
    for (int i = 12 - n; i < 12; i++) {
        round_ref(s, ascon_round_constants[i]);
    }
}

int test_perm_integration()
{
    state_t s1, s2;
    get_random_state(&s1, &s2);

    P6(&s1);
    ref_permutation(&s2, 6);
    assert(memcmp(&s1, &s2, sizeof(state_t)) == 0);

    P8(&s1);
    ref_permutation(&s2, 8);
    assert(memcmp(&s1, &s2, sizeof(state_t)) == 0);

    P12(&s1);
    ref_permutation(&s2, 12);
    assert(memcmp(&s1, &s2, sizeof(state_t)) == 0);

    P6(&s1);
    ref_permutation(&s2, 6);
    assert(memcmp(&s1, &s2, sizeof(state_t)) == 0);

    P8(&s1);
    ref_permutation(&s2, 8);
    assert(memcmp(&s1, &s2, sizeof(state_t)) == 0);

    P12(&s1);
    ref_permutation(&s2, 12);
    assert(memcmp(&s1, &s2, sizeof(state_t)) == 0);
}
