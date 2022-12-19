#include <stdint.h>

#ifdef __BI32__
    const uint64_t ascon_round_constants[] = {
        0x0000000c0000000cull,
        0x0000000c00000009ull,
        0x000000090000000cull,
        0x0000000900000009ull,
        0x0000000c00000006ull,
        0x0000000c00000003ull,
        0x0000000900000006ull,
        0x0000000900000003ull,
        0x000000060000000cull,
        0x0000000600000009ull,
        0x000000030000000cull,
        0x0000000300000009ull,
    };

    const uint32_t* ascon_round_constants_bi = (const uint32_t*)ascon_round_constants;
#else
    const uint64_t ascon_round_constants[] = {
        0xf0,
        0xe1,
        0xd2,
        0xc3,
        0xb4,
        0xa5,
        0x96,
        0x87,
        0x78,
        0x69,
        0x5a,
        0x4b,
    };
#endif
