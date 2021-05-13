#include "../../inc/mh_tests.h"
#include "../../inc/mh_handle.h"
#include "../../inc/mh_console.h"

bool program_error(MH_UNUSED mh_context_t *context, const char *message, mh_code_location_t from) {
    char loc[128];
    mh_code_location_to_string(loc, from);
    mh_write_string(MH_ERROR, "An error has occurred ");
    mh_write_string(MH_ERROR, loc);
    mh_write_string(MH_ERROR, ": ");
    mh_write_string(MH_ERROR, message);
    mh_write_string(MH_ERROR, "\n");
    exit(1);
}

void tests_print(const mh_test_t *tests, size_t count) {
    mh_test_return_t result;
    size_t failed = 0;
    for (size_t i = 0; i < count; i++) {
        mh_write_string(MH_OUTPUT, "[......] [");
        mh_write_unsigned_number(MH_OUTPUT, i+1);
        mh_write_string(MH_OUTPUT, "/");
        mh_write_unsigned_number(MH_OUTPUT, count);
        mh_write_string(MH_OUTPUT, "] Running the test `");
        mh_write_string(MH_OUTPUT, tests[i].name);
        mh_write_string(MH_OUTPUT, "`...");
        mh_tests_check(&result, &tests[i], 1);
        if (!result.success) {
            char loc[128];
            mh_code_location_to_string(loc, result.location);
            mh_write_string(MH_OUTPUT,"\r[FAILED] [");
            mh_write_unsigned_number(MH_OUTPUT, i+1);
            mh_write_string(MH_OUTPUT, "/");
            mh_write_unsigned_number(MH_OUTPUT, count);
            mh_write_string(MH_OUTPUT, "] The test `");
            mh_write_string(MH_OUTPUT, tests[i].name);
            mh_write_string(MH_OUTPUT, "` has failed because \"");
            mh_write_string(MH_OUTPUT, result.reason);
            mh_write_string(MH_OUTPUT, "\" ");
            mh_write_string(MH_OUTPUT, loc);
            mh_write_string(MH_OUTPUT, ".\n");
            failed++;
            if (tests[i].required) {
                mh_write_string(MH_ERROR, "That was a required test, stopping program...\n");
                exit(1);
            }
        } else {
            mh_write_string(MH_OUTPUT, "\r[PASSED] [");
            mh_write_unsigned_number(MH_OUTPUT, i+1);
            mh_write_string(MH_OUTPUT, "/");
            mh_write_unsigned_number(MH_OUTPUT, count);
            mh_write_string(MH_OUTPUT, "] The test `");
            mh_write_string(MH_OUTPUT, tests[i].name);
            mh_write_string(MH_OUTPUT, "` has passed.\n");
        }
    }
    mh_write_string(MH_OUTPUT, "Finished testing and ");
    if (failed) {
        mh_write_unsigned_number(MH_OUTPUT, count - failed);
        mh_write_string(MH_OUTPUT, " out of ");
        mh_write_unsigned_number(MH_OUTPUT, count);
        mh_write_string(MH_OUTPUT, " tests passed.\n");
    } else {
        mh_write_string(MH_OUTPUT, "all tests passed.\n");
    }
    exit(failed != 0);
}

int main(int argc, char *argv[]) {
    typedef mh_tests_t (*mh_test_provider_t)(void);
    mh_context_set_error_handler(MH_GLOBAL, program_error);

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
        mh_write_string(MH_OUTPUT, mh_tests_check(NULL, tests.tests, tests.count) ? "1" : "0");
    } else {
        tests_print(tests.tests, tests.count);
    }
}
