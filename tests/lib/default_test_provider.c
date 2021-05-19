#include "../../inc/mh_test_provider.h"
#include "default_tests.h"

mh_map_t *test_map;

MH_CONSTRUCTOR(200) void test_ctr(void) {
    test_map = mh_map_new(MH_GLOBAL);
}

static mh_memory_t *mem;
size_t count = 0;

mh_tests_t mh_test_provider(void) {
    mem = mh_memory_new(MH_GLOBAL, sizeof(mh_test_t), false);
    MH_FOREACH(mh_key_value_pair_t *, item, test_map) {
        if (mem->size < sizeof(mh_test_t) * (count + 1)) {
            mh_memory_resize(MH_GLOBAL, mem, sizeof(mh_test_t) * (count + 1));
        }
        mh_test_t test = {
                .required = false,
                .func = (mh_test_function_t) (size_t) item->value.address,
                .name = item->key.address
        };
        ((mh_test_t *) mem->address)[count++] = test;
    }
    return (mh_tests_t) {.count = count, .tests = mem->address};
}
