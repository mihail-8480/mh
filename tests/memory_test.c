#include "lib/default_tests.h"
#include "../inc/mh_memory.h"

MH_TEST_ADD(memory_new_test) {
    mh_memory_t *memory = mh_memory_new(MH_GLOBAL, 10, true);

    MH_TEST_EXPECT(memory != NULL);
    MH_TEST_EXPECT(memory->address != NULL);
    MH_TEST_EXPECT(memory->offset == 0);

    for (int i = 0; i < 10; i++) {
        MH_TEST_EXPECT(((char *) memory->address)[i] == 0);
    }

    MH_TEST_PASSED();
}

MH_TEST_ADD(memory_resize_test) {
    mh_memory_t *memory = mh_memory_new(MH_GLOBAL, 10, false);
    mh_memory_resize(MH_GLOBAL, memory, 20);

    MH_TEST_EXPECT(memory->address != NULL);
    MH_TEST_PASSED();
}

MH_TEST_ADD(memory_reference_test) {
    char str[] = "test";
    mh_memory_t ref = mh_memory_reference(str, 4);

    MH_TEST_EXPECT(ref.offset == 0);
    MH_TEST_EXPECT(ref.address == str);
    MH_TEST_EXPECT(ref.size == 4);
    MH_TEST_PASSED();
}

MH_TEST_ADD(memory_read_until_test) {
    char str[] = "test";
    mh_memory_t ref = mh_memory_reference(str, 4);
    mh_memory_t ref2 = mh_memory_read_until(&ref, 's');
    mh_memory_t ref3 = mh_memory_read_until(&ref, 'x');
    mh_memory_read_until(&ref, 't');
    MH_TEST_EXPECT(ref.offset == 4);
    MH_TEST_EXPECT(ref3.address == NULL);
    MH_TEST_EXPECT(ref3.size == 0);
    MH_TEST_EXPECT(ref2.size == 2);
    MH_TEST_PASSED();
}

MH_TEST_ADD(memory_index_of_test) {
    char str[] = "test";
    mh_memory_t ref = mh_memory_reference(str, 4);

    MH_TEST_EXPECT(mh_memory_index_of(ref, 'e') == 1);
    MH_TEST_PASSED();
}

MH_TEST_ADD(memory_to_string_test) {
    char str[] = "test";
    char str2[5];
    mh_memory_t ref = mh_memory_reference(str, 4);
    mh_memory_to_string(str2, ref);

    MH_TEST_EXPECT(strcmp(str, str2) == 0);
    MH_TEST_PASSED();
}

MH_TEST_ADD(memory_is_equal_test) {
    char str[] = "test";
    mh_memory_t ref = mh_memory_reference(str, 4);
    mh_memory_t ref2 = mh_memory_reference(str, 4);
    mh_memory_t ref3 = mh_memory_reference(str + 1, 3);
    mh_memory_t ref4 = mh_memory_reference(str, 3);

    MH_TEST_EXPECT(mh_memory_is_equal(ref, ref2));
    MH_TEST_EXPECT(!mh_memory_is_equal(ref, ref4));
    MH_TEST_EXPECT(!mh_memory_is_equal(ref3, ref4));
    MH_TEST_PASSED();
}
