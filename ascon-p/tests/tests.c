#include <_internal/testing.h>

int test_generic_ror();
int test_bi_ror64();
int test_s_box();
int test_linear_layer();
int test_perm_integration();

void setup_tests()
{
    // rotation tests
    TEST(test_generic_ror, "ror/generic_ror");
    TEST(test_bi_ror64, "bi32/ror64");

    // s-Box, linear layer tests
    TEST(test_s_box, "perm/s_box");
    TEST(test_linear_layer, "perm/linear_layer");

    // permutation integration tests
    TEST(test_perm_integration, "perm/integration");
}
