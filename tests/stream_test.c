#include "default_tests.h"
#include "../inc/mh_stream.h"

MH_TEST_NEW(memory_stream_test) {
    bool failed = false;
    const char* reason = "Success.";
    MH_CONTEXT(context, {
        mh_stream_t* stream = mh_memory_stream_new(context, 1, false);
        mh_memory_t* memory = mh_memory_stream_get_memory(stream);
        if (memory->address == NULL) {
            failed = true;
            reason = "Didn't allocate memory.";
            goto test_end;
        }
        if (memory->size != 1) {
            failed = true;
            reason = "Allocated the wrong amount of memory.";
            goto test_end;
        }
        size_t amount = mh_stream_write_reference(stream, "Test", 4);
        if (amount != 4) {
            failed = true;
            reason = "Couldn't write the test string.";
            goto test_end;
        }
        mh_stream_seek(stream, 0);
        char test[5];
        mh_memory_t ref = mh_memory_reference(test, 5);
        mh_stream_read(stream, &ref, 4);
        if (strcmp(test, "Test") != 0) {
            failed = true;
            reason = "The value that was read from the stream is incorrect.";
            goto test_end;
        }
    })
    test_end:
    MH_TEST_RESULT(!failed, reason);
}
