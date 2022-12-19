#include <_internal/testing.h>
#include <_internal/printf.h>

int tests_passed = 0;
int tests_failed = 0;

extern void setup_tests();

void testing_run_test(int(*t)(void), const char* name)
{
    int result = t();

    printf(COLOR_CYAN "%s... ", name);
    if (result == TEST_SUCCESS)
    {
        tests_passed++;
        printf(COLOR_YELLOW " [passed]");
    }
    else if (result == TEST_SKIPPED)
    {
        printf(COLOR_YELLOW " [skipped]");
    }
    else
    {
        tests_failed++;
        printf(COLOR_RED " [failed]");
    }

    printf(COLOR_RESET "\n");
}

void testing_run_bench(void(*b)(int), const char* name)
{
    printf(COLOR_CYAN "%s" COLOR_RESET ": ", name);
    for (int msg_len = 16; msg_len <= 16*1024; msg_len *= 2)
    {
        uint64_t before = get_cycles();
        b(msg_len);
        uint64_t after = get_cycles();

        uint64_t cycles = after - before;
        float cpb = (float)cycles / (float)msg_len;
        
        printf("%d/%.1f, ", msg_len, cpb);
    }
    printf("\n");
}

int main()
{
    printf("Running tests...\n\n");

    setup_tests();

    printf("\n\n%d tests passed, %d tests failed\n", tests_passed, tests_failed);

    if (tests_failed > 0) {
        return TEST_FAILURE;
    }
    return TEST_SUCCESS;
}
