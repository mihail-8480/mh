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

        MH_TEST(handle_test),

        MH_TEST(context_allocate_test),
        MH_TEST(context_create_test),
        MH_TEST(context_destructor_test),
        MH_TEST(context_error_test),
        MH_TEST(context_reallocate_test),
        MH_TEST(context_thread_bind_test),

        MH_TEST(memory_stream_test),


        MH_TEST(collection_iterator_test),
        MH_TEST(collection_map_test),
        MH_TEST(collection_list_test),
        MH_TEST(collection_stack_test),
        MH_TEST(collection_find_test),
        MH_TEST(collection_find_test_map),

        MH_TEST(tcp_test),

        MH_TEST(conversion_test)
};

mh_tests_t mh_test_provider(void) {
    return (mh_tests_t) {.count = MH_FIXED_COUNT(tests), .tests = tests};
}
