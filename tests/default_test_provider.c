#include "../inc/mh_test_provider.h"
#include "default_tests.h"

static const mh_test_t tests[] = {
        MH_TEST(thread_test),

        MH_TEST(memory_new_test),
        MH_TEST(memory_resize_test),
        MH_TEST(memory_reference_test),
        MH_TEST(memory_read_until_test),
        MH_TEST(memory_index_of_test),
        MH_TEST(memory_to_string_test),
        MH_TEST(memory_is_equal_test),

        MH_TEST(handle_test)
};

mh_tests_t mh_test_provider(void){
    return (mh_tests_t) {.count = MH_FIXED_COUNT(tests), .tests = tests};
}
