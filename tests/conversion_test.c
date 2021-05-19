#include "lib/default_tests.h"
#include "../inc/mh_convert.h"

MH_TEST_ADD(conversion_test) {
    char str[5];
    char zero[2];
    mh_memory_t mem = MH_REF_CONST(str);
    mh_memory_t zbuf = MH_REF_CONST(zero);
    mh_unsigned_number_t u_num = 0;
    mh_signed_number_t num = 0;

    MH_TEST_EXPECT(!mh_int_to_string(&mem, -1, 0));
    MH_TEST_EXPECT(!mh_uint_to_string(&mem, -1, 1));

    MH_TEST_EXPECT(mh_uint_to_string(&zbuf, 0, MH_BASE_DEC));
    MH_TEST_EXPECT(*zero == '0');

    MH_TEST_EXPECT(mh_int_to_string(&zbuf, 0, MH_BASE_DEC));
    MH_TEST_EXPECT(*zero == '0');

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

    MH_TEST_EXPECT(mh_uint_parse("123", MH_BASE_DEC, &u_num));
    MH_TEST_EXPECT(u_num == 123);

    MH_TEST_EXPECT(mh_int_parse("123", MH_BASE_DEC, &num));
    MH_TEST_EXPECT(num == 123);

    MH_TEST_EXPECT(mh_int_parse("-123", MH_BASE_DEC, &num));
    MH_TEST_EXPECT(num == -123);

    MH_TEST_EXPECT(mh_int_parse("+123", MH_BASE_DEC, &num));
    MH_TEST_EXPECT(num == 123);

    MH_TEST_EXPECT(mh_int_parse("0", MH_BASE_DEC, &num));
    MH_TEST_EXPECT(num == 0);

    MH_TEST_EXPECT(mh_int_parse("-0", MH_BASE_DEC, &num));
    MH_TEST_EXPECT(num == 0);

    MH_TEST_EXPECT(mh_int_parse("+0", MH_BASE_DEC, &num));
    MH_TEST_EXPECT(num == 0);

    MH_TEST_EXPECT(mh_uint_parse("0", MH_BASE_DEC, &u_num));
    MH_TEST_EXPECT(u_num == 0);

    MH_TEST_EXPECT(mh_int_parse("beef", MH_BASE_HEX, &num));
    MH_TEST_EXPECT(num == 0xbeef);

    MH_TEST_EXPECT(mh_int_parse("10", MH_BASE_BIN, &num));
    MH_TEST_EXPECT(num == 2);

    MH_TEST_EXPECT(!mh_int_parse("2", MH_BASE_BIN, &num));

    MH_TEST_PASSED();
}
