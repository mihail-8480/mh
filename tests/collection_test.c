#include "default_tests.h"
#include "../inc/mh_map.h"

MH_TEST_CONTEXT_NEW(map_test, {
    mh_map_t* map = mh_map_new(context);
    char key[] = "TestKey";
    char invalid_key[] = "TestKeyInvalid";
    char value[] = "TestValue";
    mh_map_add(map, MH_REF_CONST(key), MH_REF_CONST(value));
    MH_TEST_CONTEXT_EXPECT(!mh_map_contains(map, MH_REF_CONST(invalid_key)))
    MH_TEST_CONTEXT_EXPECT(mh_map_contains(map, MH_REF_CONST(key)))
    MH_TEST_CONTEXT_EXPECT(mh_memory_is_equal(mh_map_get(map, MH_REF_CONST(key)), MH_REF_CONST(value)))
})
