#include "tests.h"
#include "../inc/mh_memory.h"
MH_TEST_NEW(memory_new_test) {
    bool fail = false;
    MH_CONTEXT(context, {
        mh_memory_t* memory = mh_memory_new(context, 10, true);
        if (memory == NULL || memory->address == NULL || memory->offset != 0) {
            fail = true;
        } else {
            for (int i = 0; i < 10; i++) {
                if (((char *) memory->address)[i] != 0) {
                    fail = true;
                    break;
                }
            }
        }
    })
    MH_TEST_RESULT(!fail, "Allocated memory is not zeroed.");
}

MH_TEST_NEW(memory_resize_test) {
    bool fail = false;
    MH_CONTEXT(context, {
        mh_memory_t* memory = mh_memory_new(context, 10, false);
        mh_memory_resize(context, memory, 20);
        if (memory->address == NULL) {
            fail = true;
        }
    })
    MH_TEST_RESULT(!fail, "The returned memory address is NULL.");
}

MH_TEST_NEW(memory_reference_test) {
    char str[] = "test";
    mh_memory_t ref = mh_memory_reference(str, 4);

    MH_TEST_EXPECT(ref.offset == 0);
    MH_TEST_EXPECT(ref.address == str);
    MH_TEST_EXPECT(ref.size == 4);
    MH_TEST_PASSED();
}

MH_TEST_NEW(memory_read_until_test) {
    char str[] = "test";
    mh_memory_t ref = mh_memory_reference(str, 4);
    mh_memory_t ref2 = mh_memory_read_until(&ref, 's');
    mh_memory_t ref3 = mh_memory_read_until(&ref, 'x');


    MH_TEST_EXPECT(ref.offset == 3);
    MH_TEST_EXPECT(ref3.address == NULL);
    MH_TEST_EXPECT(ref3.size == 0);
    MH_TEST_EXPECT(ref2.size == 2);
    MH_TEST_PASSED();
}

MH_TEST_NEW(memory_index_of_test) {
    char str[] = "test";
    mh_memory_t ref = mh_memory_reference(str, 4);

    MH_TEST_EXPECT(mh_memory_index_of(ref, 'e') == 1);
    MH_TEST_PASSED();
}

MH_TEST_NEW(memory_to_string_test) {
    char str[] = "test";
    char str2[5];
    mh_memory_t ref = mh_memory_reference(str, 4);
    mh_memory_to_string(str2, ref);

    MH_TEST_EXPECT(strcmp(str, str2) == 0);
    MH_TEST_PASSED();
}

MH_TEST_NEW(memory_is_equal_test) {
    char str[] = "test";
    mh_memory_t ref = mh_memory_reference(str, 4);
    mh_memory_t ref2 = mh_memory_reference(str, 4);
    mh_memory_t ref3 = mh_memory_reference(str+1, 3);
    mh_memory_t ref4 = mh_memory_reference(str, 3);
    MH_TEST_EXPECT(mh_memory_is_equal(ref, ref2));
    MH_TEST_EXPECT(!mh_memory_is_equal(ref, ref4));
    MH_TEST_EXPECT(!mh_memory_is_equal(ref3, ref4));
    MH_TEST_PASSED();
}
