#include "default_tests.h"
#include "../inc/mh_map.h"
MH_TEST_NEW(map_test) {
    bool failed = false;
    const char* reason = "Success.";

    MH_CONTEXT(context, {
        mh_map_t* map = mh_map_new(context);
        char key[] = "TestKey";
        char invalid_key[] = "TestKeyInvalid";
        char value[] = "TestValue";
        mh_map_add(map, mh_memory_reference(key, MH_FIXED_COUNT(key)), mh_memory_reference(value, MH_FIXED_COUNT(value)));
        if (mh_map_contains(map, mh_memory_reference(invalid_key, MH_FIXED_COUNT(invalid_key)))) {
            failed = true;
            reason = "mh_map_contains() returned true for an invalid key.";
            goto test_end;
        }
        if (!mh_map_contains(map, mh_memory_reference(key, MH_FIXED_COUNT(key)))) {
            failed = true;
            reason = "mh_map_contains() returned false for a key that was supposed to be inside.";
            goto test_end;
        }
        mh_memory_t returned_value = mh_map_get(map, mh_memory_reference(key, MH_FIXED_COUNT(key)));
        if (!mh_memory_is_equal(returned_value, mh_memory_reference(value, MH_FIXED_COUNT(value)))) {
            failed = true;
            reason = "mh_map_get() returned a value that isn't the same as expected.";
            goto test_end;
        }
    })
    test_end:
    MH_TEST_RESULT(!failed, reason);
}
