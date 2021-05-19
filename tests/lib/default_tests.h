#ifndef MH_TESTS_LIST_H
#define MH_TESTS_LIST_H

#include "../../inc/mh_tests.h"
#include "../../inc/mh_map.h"

extern mh_map_t *test_map;

#define MH_TEST_ADD(T) \
MH_TEST_NEW(T); \
MH_CONSTRUCTOR(201) void (T##_ctr)(void) { \
    mh_map_set(test_map, MH_STRING(#T), MH_REF_CONST((mh_ref_t)(size_t)T)); \
} \
MH_TEST_NEW(T)

#endif //MH_TESTS_LIST_H
