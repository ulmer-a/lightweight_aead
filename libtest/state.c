#include <_internal/state.h>

void get_random_state(state_t* s1, state_t* s2)
{
    state_t tmp = {
        .x = {
            0xdeadbeefbadeaffe,
            0xbadeaffedeadbeef,
            0xabcdefabcdefabcd,
            0xfedcbafdaffebade,
            0xab01203040489375
        }
    };
    *s1 = tmp;
    *s2 = tmp;
}
