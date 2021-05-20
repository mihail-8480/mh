#include "../../../inc/mh_tests.h"
#include "../../../inc/mh_handle.h"
#include "../../../inc/mh_console.h"

void tests_print(const mh_test_t *tests, size_t count) {
    mh_test_return_t result;
    size_t failed = 0;
    for (size_t i = 0; i < count; i++) {
        MH_WRITE("[......] [{}/{}] Running the test `{}`...", MH_FMT_INT(i + 1), MH_FMT_INT(count),
                 MH_FMT_STR(tests[i].name));
        mh_tests_check(&result, &tests[i], 1);
        if (!result.success) {
            MH_WRITE("\r[FAILED] [{}/{}] The test `{}` has failed because \"{}\" {}.\n", MH_FMT_INT(i + 1),
                     MH_FMT_INT(count), MH_FMT_STR(tests[i].name), MH_FMT_STR(result.reason),
                     MH_FMT_LOC(&result.location));
            failed++;
            if (tests[i].required) {
                MH_WRITE_ERR("That was a required test, stopping program...\n");
                exit(1);
            }
        } else {
            MH_WRITE("\r[PASSED] [{}/{}] The test `{}` has passed.\n", MH_FMT_INT(i + 1), MH_FMT_INT(count),
                     MH_FMT_STR(tests[i].name));
        }
    }
    if (failed) {
        MH_WRITE("Finished testing and {} out of {} tests passed.\n", MH_FMT_INT(count - failed), MH_FMT_INT(count));
    } else {
        MH_WRITE("Finished testing and all tests passed.\n");
    }
    exit(failed != 0);
}

int main(int argc, char *argv[]) {
    typedef mh_tests_t (*mh_test_provider_t)(void);
    mh_argument_parser_args_t args = {
            .required_arguments = "lib",
            .optional_arguments = "check_only",
    };

    mh_map_t *map = mh_argument_parse(MH_GLOBAL, &args, argc, argv);
    mh_memory_t lib = mh_map_get(map, MH_STRING("lib"));
    mh_memory_t check_only_s = mh_map_get(map, MH_STRING("check_only"));

    bool check_only = check_only_s.address != NULL && strcmp(MH_MEM_TO_STRING(check_only_s), "check") == 0;

    mh_handle_t *handle = mh_handle_new(MH_GLOBAL, MH_MEM_TO_STRING(lib));
    mh_test_provider_t provider = (mh_test_provider_t) (size_t) mh_handle_find_symbol(handle, "mh_test_provider");
    mh_tests_t tests = provider();
    if (check_only) {
        MH_WRITE("{}", MH_FMT_BOOL(mh_tests_check(NULL, tests.tests, tests.count)));
    } else {
        tests_print(tests.tests, tests.count);
    }
}
