#include "default_tests.h"
#include "../inc/mh_map.h"

static int c = 0;

static void count(MH_UNUSED mh_memory_t mem) {
    c++;
}

MH_TEST_NEW(map_test) {
    mh_map_t *map = mh_map_new(MH_GLOBAL);
    char key[] = "TestKey";
    char key2[] = "TestKey1";
    char invalid_key[] = "TestKeyInvalid";
    char value[] = "TestValue";
    char value2[] = "TestValue2";
    mh_map_add(map, MH_REF_CONST(key), MH_REF_CONST(value));
    mh_map_add(map, MH_REF_CONST(key2), MH_REF_CONST(value2));
    mh_collection_foreach(&map->collection, count);
    MH_TEST_EXPECT(c == 2);
    MH_TEST_EXPECT(!mh_map_contains(map, MH_REF_CONST(invalid_key)));
    MH_TEST_EXPECT(mh_map_contains(map, MH_REF_CONST(key)));
    MH_TEST_EXPECT(mh_memory_is_equal(mh_map_get(map, MH_REF_CONST(key)), MH_REF_CONST(value)));
    MH_TEST_PASSED();
}
