#include "../../inc/mh_test_provider.h"
#include "default_tests.h"

mh_map_t *test_map;
static mh_memory_t *mem;
static size_t count = 0;

MH_CONSTRUCTOR(200) void test_ctr(void) {
    test_map = mh_map_new(MH_GLOBAL);
}

MH_CONSTRUCTOR(202) void arr_ctr(void) {
    mem = mh_memory_new(MH_GLOBAL, sizeof(mh_test_t), false);
    MH_FOREACH(mh_key_value_pair_t *, item, test_map) {
        if (mem->size < sizeof(mh_test_t) * (count + 1)) {
            mh_memory_resize(MH_GLOBAL, mem, sizeof(mh_test_t) * (count + 1));
        }
        mh_test_t test = {
                .required = false,
                .func = MH_FORCE_CAST_PTR(mh_test_function_t,item->value.address),
                .name = item->key.address
        };
        ((mh_test_t *) mem->address)[count++] = test;
    }
}

mh_tests_t mh_test_provider(void) {
    return (mh_tests_t) {.count = count, .tests = mem->address};
}
