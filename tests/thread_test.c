#include "tests.h"
#include "../inc/mh_thread.h"
#include <unistd.h>

static void* modify(void* args) {
    bool* arg_bool = (bool*)args;
    *arg_bool = false;
    return NULL;
}

MH_TEST_NEW(thread_test) {
    bool args = true;
    mh_thread_create(modify, &args);
    size_t c = 0;
    while(args) {
        if (c >= 10) {
            MH_TEST_FAIL();
        }
        usleep(100);
        c++;
    }
    MH_TEST_PASSED();
}
