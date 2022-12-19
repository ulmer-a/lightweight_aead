#include <_internal/testing.h>

#if defined(__RV32__)

    #if defined(SYS_VERILATOR)

    uint64_t get_cycles()
    {
        uint32_t cycles_lo, cycles_hi;
        __asm__ volatile (
            "csrr %0, 0xb00;"
            "csrr %1, 0xb80;"
            : "=r"(cycles_lo), "=r"(cycles_hi)
        );
        return ((uint64_t)cycles_hi << 32) | cycles_lo;
    }

    #else

    uint64_t get_cycles()
    {
        uint32_t cycles_lo, cycles_hi;
        __asm__ volatile (
            "rdcycle %0;"
            "rdcycleh %1;"
            : "=r"(cycles_lo), "=r"(cycles_hi)
        );
        return ((uint64_t)cycles_hi << 32) | cycles_lo;
    }

    #endif

#elif defined(__RV64__)

    uint64_t get_cycles()
    {
        uint64_t cycles;
        __asm__ volatile (
            "rdcycle %0;"
            : "=r"(cycles)
        );
        return cycles;
    }

#elif defined(__X86__)

    uint64_t get_cycles()
    {
        return __builtin_ia32_rdtsc();
    }

#elif defined(__X86_64__)

    uint64_t get_cycles()
    {
        return __builtin_x86_64_rdtsc();
    }

#else
    #error get_cycles() unimplemented
#endif