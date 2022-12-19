#if defined(TEST_FREESTANDING)

    extern int main();

    #if defined(__RV32__) || defined(__RV64__)

        #if !defined(SYS_VERILATOR)

        void __attribute__((noreturn)) exit(int status)
        {
            register unsigned long a0 asm("a0") = status;
            register unsigned long id asm("a7") = 93;

            __asm__ volatile (
                "ecall" : :
                "r"(a0),
                "r"(id)
            );

            __builtin_unreachable();
        }

        void _putchar(char character)
        {
            static char c;
            c = character;

            register int fd asm("a0") = 1;
            register char* ptr asm("a1") = &c;
            register unsigned long len asm("a2") = 1;
            register unsigned long id asm("a7") = 64;

            __asm__ volatile (
                "ecall" : :
                "r"(fd),
                "r"(ptr),
                "r"(len),
                "r"(id)
            );
        }

        #endif

    #else
        #error putchar() and exit() not implemented for target architecture
    #endif // #if defined(__RV32__) || defined(__RV64__)

    int memcmp(const void* str1, const void* str2, unsigned long count)
    {
        register const unsigned char *s1 = (const unsigned char*)str1;
        register const unsigned char *s2 = (const unsigned char*)str2;

        while (count-- > 0)
        {
            if (*s1++ != *s2++)
                return s1[-1] < s2[-1] ? -1 : 1;
        }

        return 0;
    }

    void* memcpy(void* dest, const void* src, unsigned long n)
    {
        void* dest_ptr = dest;
        while (n--)
        {
            *(unsigned char*)dest++ = *(unsigned char*)src++;
        }
        return dest_ptr;
    }

    #if !defined(SYS_VERILATOR)

    void _start() {
        __asm__ volatile (
            ".option push;"
            ".option norelax;"
            "la gp, __global_pointer$;"
            ".option pop;"
        );

        int return_value = main();
        exit(return_value);
    }

    #endif

#endif // #if defined(TEST_FREESTANDING)
