#pragma once

#include <stdint.h>
#include <alloca.h>
#include <_internal/state.h>

#define TEST_SUCCESS 0
#define TEST_FAILURE 1
#define TEST_SKIPPED 2

#if defined(SYS_VERILATOR)
    #define COLOR_CYAN
    #define COLOR_YELLOW
    #define COLOR_RED
    #define COLOR_RESET
#else
    #define COLOR_CYAN      "\033[0;36m"
    #define COLOR_YELLOW    "\033[0;33m"
    #define COLOR_RED       "\033[0;31m"
    #define COLOR_RESET     "\033[0m"
#endif

#if defined(TEST_FREESTANDING)
    #include <_internal/printf.h>
    #define memcmp __builtin_memcmp
    extern void exit(int status);
#else
    #include <stdio.h>
    #include <string.h>
#endif

#define assert(x) if (!(x)) {                       \
    printf(COLOR_RED                                \
           "assertion failed: %s:%d"                \
           COLOR_RESET "\n", __FILE__, __LINE__);   \
    return TEST_FAILURE;                            \
}

extern void testing_run_test(int(*t)(void), const char* name);
#define TEST(t, name) testing_run_test(t, name)

extern void testing_run_bench(void(*b)(int), const char* name);
#define BENCH(b, name) testing_run_bench(b, name)

void get_random_state(state_t* s1, state_t* s2);

uint64_t get_cycles();

typedef struct {
    const unsigned char     *key;
    const unsigned char     *npub;
    const unsigned char     *ad;
    const unsigned long long ad_size;
    const unsigned char     *pt;
    const unsigned long long pt_size;
    const unsigned char     *ct;
    const unsigned long long ct_size;
    const unsigned char     *tag;
} testcase_t;

extern unsigned char __RANDOM__[];
