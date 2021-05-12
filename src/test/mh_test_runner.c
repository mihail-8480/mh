#include "../../inc/mh_tests.h"
#include "../../inc/mh_handle.h"
#include "../../inc/mh_console.h"
#include "../../inc/mh_convert.h"

bool program_error(MH_UNUSED mh_context_t *context, const char *message, mh_code_location_t from) {
    char loc[128];
    mh_code_location_to_string(loc, from);
    MH_PRINT_ERROR("An error has occurred ");
    MH_PRINT_ERROR_STR(loc);
    MH_PRINT_ERROR(": ");
    MH_PRINT_ERROR_STR(message);
    MH_PRINT_ERROR("\n");
    exit(1);
}

void tests_print(const mh_test_t *tests, size_t count) {
    mh_test_return_t result;
    size_t failed = 0;
    char conv_buf[20];
    mh_memory_t conv = MH_REF_CONST(conv_buf);
    for (size_t i = 0; i < count; i++) {
        MH_PRINT("[......] [");
        mh_uint_to_string(&conv, i+1, MH_BASE_DEC);
        MH_PRINT_STR(conv.address);
        MH_PRINT("/");
        mh_uint_to_string(&conv, count, MH_BASE_DEC);
        MH_PRINT_STR(conv.address);
        MH_PRINT("] Running the test `");
        MH_PRINT_STR(tests[i].name);
        MH_PRINT("`...");
        fflush(stdout);
        mh_tests_check(&result, &tests[i], 1);
        if (!result.success) {
            char loc[128];
            mh_code_location_to_string(loc, result.location);
            MH_PRINT("\r[FAILED] [");
            mh_uint_to_string(&conv, i+1, MH_BASE_DEC);
            MH_PRINT_STR(conv.address);
            MH_PRINT("/");
            mh_uint_to_string(&conv, count, MH_BASE_DEC);
            MH_PRINT_STR(conv.address);
            MH_PRINT("] The test `");
            MH_PRINT_STR(tests[i].name);
            MH_PRINT("` has failed because \"");
            MH_PRINT_STR(result.reason);
            MH_PRINT("\" ");
            MH_PRINT_STR(loc);
            MH_PRINT(".\n");
            failed++;
            if (tests[i].required) {
                MH_PRINT_ERROR("That was a required test, stopping program...\n");
                exit(1);
            }
        } else {
            MH_PRINT("\r[PASSED] [");
            mh_uint_to_string(&conv, i+1, MH_BASE_DEC);
            MH_PRINT_STR(conv.address);
            MH_PRINT("/");
            mh_uint_to_string(&conv, count, MH_BASE_DEC);
            MH_PRINT_STR(conv.address);
            MH_PRINT("] The test `");
            MH_PRINT_STR(tests[i].name);
            MH_PRINT("` has passed.\n");
        }
    }
    MH_PRINT("Finished testing and ");
    if (failed) {
        mh_uint_to_string(&conv, count - failed, MH_BASE_DEC);
        MH_PRINT_STR(conv.address);
        MH_PRINT(" out of ");
        mh_uint_to_string(&conv, count, MH_BASE_DEC);
        MH_PRINT_STR(conv.address);
        MH_PRINT(" tests passed.\n");
    } else {
        MH_PRINT("all tests passed.\n");
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
        mh_stream_write_reference(MH_OUT, mh_tests_check(NULL, tests.tests, tests.count) ? "1" : "0", 1);
    } else {
        tests_print(tests.tests, tests.count);
    }
}
