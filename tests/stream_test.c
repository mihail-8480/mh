#include "lib/default_tests.h"
#include "../inc/mh_stream.h"

MH_TEST_ADD(memory_stream_test) {
    mh_stream_t *stream = mh_memory_stream_new(MH_GLOBAL, 1, false);
    mh_memory_t *memory = mh_memory_stream_get_memory(stream);

    MH_TEST_EXPECT(memory->address != NULL);
    MH_TEST_EXPECT(memory->size == 1);
    MH_TEST_EXPECT(mh_stream_write_reference(stream, "Test", 4) == 4);

    mh_stream_seek(stream, 0);
    char test[5] = "Null";
    mh_memory_t ref = mh_memory_reference(test, 5);
    mh_stream_read(stream, &ref, 4);

    MH_TEST_EXPECT(strcmp(test, "Test") == 0);
    MH_TEST_PASSED();
}
