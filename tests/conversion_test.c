#include "lib/default_tests.h"
#include "../inc/mh_convert.h"

MH_TEST_NEW(conversion_test) {
    char str[5];
    mh_memory_t mem = MH_REF_CONST(str);

    MH_TEST_EXPECT(mh_uint_to_string(&mem, 5000, MH_BASE_DEC));
    MH_TEST_EXPECT(mh_memory_is_equal(mem, MH_REF_CONST("5000")));

    MH_TEST_EXPECT(mh_int_to_string(&mem, -420, MH_BASE_DEC));
    MH_TEST_EXPECT(mh_memory_is_equal(mem, MH_REF_CONST("-420")));

    MH_TEST_EXPECT(mh_int_to_string(&mem, 1337, MH_BASE_DEC));
    MH_TEST_EXPECT(mh_memory_is_equal(mem, MH_REF_CONST("1337")));

    MH_TEST_EXPECT(mh_int_to_string(&mem, 15, MH_BASE_BIN));
    MH_TEST_EXPECT(mh_memory_is_equal(mem, MH_REF_CONST("1111")));

    MH_TEST_EXPECT(mh_uint_to_string(&mem, 0xbeef, MH_BASE_HEX));
    MH_TEST_EXPECT(mh_memory_is_equal(mem, MH_REF_CONST("beef")));

    MH_TEST_PASSED();
}
