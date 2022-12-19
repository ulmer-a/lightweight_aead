#include <_internal/testing.h>
#include <_internal/common.h>

#include "../word.h"

#if defined(__RV32__) && defined(__BI32__)

    int u64big_test()
    {
        #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

            assert(U64BIG(0xdeadbeef) == 0xefbeadde00000000);
            assert(U64BIG(0xbadedeadbeefaffe) == 0xfeafefbeaddedeba);

        #elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

            assert(U64BIG(0xdeadbeef) == 0xdeadbeef);
            assert(U64BIG(0xbadedeadbeefaffe) == 0xbadedeadbeefaffe);

        #else
            #error neither little nor big endian macros defined!
        #endif

        return TEST_SUCCESS;
    }

    int interleave_cons_test()
    {
        assert(
            deinterleave32(interleave32(0xdeadbeefbadeaffe))
            == 0xdeadbeefbadeaffe
        );
        assert(
            interleave32(deinterleave32(0xdeadbadeaffebeef))
            == 0xdeadbadeaffebeef
        );
        return TEST_SUCCESS;
    }

    #ifdef HAVE_ZBKB

        int bi32_interleave_16_test()
        {
            assert(interleave16(0xdeadbeef) == interleave16_ref(0xdeadbeef));
            assert(interleave16(0xbadeaffe) == interleave16_ref(0xbadeaffe));

            return TEST_SUCCESS;
        }

        int bi32_deinterleave_16_test()
        {
            assert(deinterleave16(0xdeadbeef) == deinterleave16_ref(0xdeadbeef));
            assert(deinterleave16(0xbadeaffe) == deinterleave16_ref(0xbadeaffe));

            return TEST_SUCCESS;
        }

        int bi32_interleave_32_test()
        {
            assert(interleave32(0xdeadbeefbadeaffe) == interleave32_ref(0xdeadbeefbadeaffe));
            assert(interleave32(0xbadeaffebadeaffe) == interleave32_ref(0xbadeaffebadeaffe));

            return TEST_SUCCESS;
        }

        int bi32_deinterleave_32_test()
        {
            assert(deinterleave32(0xdeadbeefbadeaffe) == deinterleave32_ref(0xdeadbeefbadeaffe));
            assert(deinterleave32(0xbadeaffebadeaffe) == deinterleave32_ref(0xbadeaffebadeaffe));

            return TEST_SUCCESS;
        }

    #else
        #define SKIP_INTERLEAVE_TESTS
    #endif

#else

    #define SKIP_INTERLEAVE_TESTS

    int u64big_test() { return TEST_SKIPPED; }
    int interleave_cons_test() { return TEST_SKIPPED; }

#endif

#if defined(SKIP_INTERLEAVE_TESTS)

    int bi32_interleave_16_test() { return TEST_SKIPPED; }
    int bi32_deinterleave_16_test() { return TEST_SKIPPED; }
    int bi32_interleave_32_test() { return TEST_SKIPPED; }
    int bi32_deinterleave_32_test() { return TEST_SKIPPED; }

#endif
