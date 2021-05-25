#include "lib/default_tests.h"
#include "../inc/mh_thread.h"
#include <unistd.h>

static mh_ref_t modify(mh_ref_t args) {
    bool *arg_bool = (bool *) args;
    *arg_bool = false;
    return NULL;
}

MH_TEST_ADD(thread_test) {
    bool args = true;
    mh_thread_create(modify, &args);
    size_t c = 0;
    while (args) {
        MH_TEST_EXPECT(c < 10);
        usleep(1000);
        c++;
    }
    MH_TEST_PASSED();
}
