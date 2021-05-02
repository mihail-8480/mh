#include "tests.h"
#include "../inc/mh_handle.h"

typedef mh_version_t (*mh_get_version_t)(void);

MH_TEST_NEW(handle_test) {
    bool correct_result = false;
    MH_CONTEXT(context, {
        mh_handle_t* handle = mh_handle_new(context, "libmh.so");
        mh_get_version_t libmh_get_version = (mh_get_version_t)(size_t)mh_handle_find_symbol(handle, "mh_get_version");
        mh_version_t version = libmh_get_version();
        unsigned int res = version.patch + version.minor + version.major;
        correct_result = res > 0;
    })
    MH_TEST_EXPECT(correct_result == true);
    MH_TEST_PASSED();
}
