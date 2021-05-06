#include "default_tests.h"
#include "../inc/mh_handle.h"

typedef mh_version_t (*mh_get_version_t)(void);

#ifdef WIN32
static const char* libmh_lib = "libmh.dll";
#else
static const char* libmh_lib = "libmh.so";
#endif

MH_TEST_CONTEXT_NEW(handle_test, {
    mh_handle_t *handle = mh_handle_new(context, libmh_lib);
    mh_get_version_t libmh_get_version = (mh_get_version_t) (size_t) mh_handle_find_symbol(handle, "mh_get_version");
    mh_version_t version = libmh_get_version();
    MH_TEST_CONTEXT_EXPECT(version.patch + version.minor + version.major > 0);
})
