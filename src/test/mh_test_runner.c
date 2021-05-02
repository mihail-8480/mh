#include "../../inc/mh_tests.h"
#include "../../inc/mh_handle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool error(MH_UNUSED mh_context_t *context, const char *message, mh_code_location_t from) {
    char loc[128];
    mh_code_location_to_string(loc, from);
    fprintf(stderr, "[program_error] An error has occurred %s: %s\n", loc, message);
    exit(1);
}

inline static const char* mh_env_default(const char* env, const char* def) {
    const char *res = getenv(env);
    if (res != NULL) {
        return res;
    }
    return def;
}

int main(int argc, char* argv[]) {
    typedef mh_tests_t (*mh_test_provider_t)(void);
    MH_CONTEXT(context, {
        mh_context_set_error_handler(context, error);

        if (argc != 2) {
            mh_context_error(context, "Invalid syntax.", MH_LOCATION_ANY());
        }

        bool check_only = strcmp(mh_env_default("MH_CHECK_ONLY", "0"), "1") == 0 ? true : false;

        mh_handle_t *handle = mh_handle_new(context, argv[1]);
        mh_test_provider_t provider = (mh_test_provider_t)(size_t)mh_handle_find_symbol(handle, "mh_test_provider");
        mh_tests_t tests = provider();
        if (check_only) {
            printf("%d", mh_tests_check(NULL, tests.tests, tests.count));
        } else {
            mh_tests_run(tests.tests, tests.count);
        }
    })
}
