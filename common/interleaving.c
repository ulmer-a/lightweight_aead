#include <stdint.h>

#if defined(__RV32__) && defined(__BI32__)
    #if defined(TESTING) || defined(NEED_REF_INTERLEAVE)

        #include <stdint.h>

        // only required for non-ZBKB RV32 and for testing!
        const uint32_t B[3] = {
            0x22222222, 0x0c0c0c0c, 0x00f000f0
        };

    #endif
#endif
