#pragma once

// Note that the output registers of the s-box inline assembly
// are actually shuffled up in order to allow optimal usage
// of the registers. Therefore:
// TODO: The register clobbers in the inline assembly statements
// are currently all set to in&out. Maybe we could make the above
// circumstance immediately visible from the code by defining
// separate input and output register clobbers, that make it clear
// which output is going to land in which register.

#if defined(__RV__) && defined(RV_HAVE_LOGIC_WITH_NEGATE)

    #define RV_S_BOX(x0, x1, x2, x3, x4, t0, t1, t2, x3_out)    \
        __asm__ volatile (                                      \
            "xor %5, %1, %2;"                                   \
            "xor %6, %0, %4;"                                   \
            "xor %7, %3, %4;"                                   \
            "orn %4, %3, %4;"                                   \
            "xor %4, %4, %5;"                                   \
            "xor %3, %3, %1;"                                   \
            "or %3, %3, %5;"                                    \
            "xor %3, %3, %6;"                                   \
            "xor %2, %2, %6;"                                   \
            "or %2, %2, %1;"                                    \
            "xor %2, %2, %7;"                                   \
            "andn %1, %1, %6;"                                  \
            "xor %1, %1, %7;"                                   \
            "or %0, %0, %7;"                                    \
            "xor %8, %0, %5;"                                   \
            : "+r"(x0), "+r"(x1), "+r"(x2), "+r"(x3), "+r"(x4), \
            "+r"(t0), "+r"(t1), "+r"(t2), "+r"(x3_out)          \
        )

#elif defined(__RV__)

    #define RV_S_BOX(x0, x1, x2, x3, x4, t0, t1, t2, x3_out)    \
        __asm__ volatile (                                      \
            "xor %5, %1, %2;"                                   \
            "xor %6, %0, %4;"                                   \
            "xor %7, %3, %4;"                                   \
            "not %4, %4;"                                       \
            "or %4, %4, %3;"                                    \
            "xor %4, %4, %5;"                                   \
            "xor %3, %3, %1;"                                   \
            "or %3, %3, %5;"                                    \
            "xor %3, %3, %6;"                                   \
            "xor %2, %2, %6;"                                   \
            "or %2, %2, %1;"                                    \
            "xor %2, %2, %7;"                                   \
            "not %6, %6;"                                       \
            "and %1, %1, %6;"                                   \
            "xor %1, %1, %7;"                                   \
            "or %0, %0, %7;"                                    \
            "xor %8, %0, %5"                                    \
            : "+r"(x0), "+r"(x1), "+r"(x2), "+r"(x3), "+r"(x4), \
            "+r"(t0), "+r"(t1), "+r"(t2), "+r"(x3_out)          \
        )

#else

    #error S-Box currently unimplemented for non-RISC-V architectures

#endif
