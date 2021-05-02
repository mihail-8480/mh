#ifndef MH_TESTS_LIST_H
#define MH_TESTS_LIST_H
#include "../inc/mh_tests.h"

MH_TEST_NEW(thread_test);

MH_TEST_NEW(memory_new_test);
MH_TEST_NEW(memory_resize_test);
MH_TEST_NEW(memory_reference_test);
MH_TEST_NEW(memory_read_until_test);
MH_TEST_NEW(memory_index_of_test);
MH_TEST_NEW(memory_to_string_test);
MH_TEST_NEW(memory_is_equal_test);

MH_TEST_NEW(handle_test);

// TODO: Add tests for the following headers: mh_context.h, mh.h, mh_stream.h, mh_collection.h, mh_map.h


#endif //MH_TESTS_LIST_H