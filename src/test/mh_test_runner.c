#include "../../inc/mh_tests.h"
#include "../../inc/mh_handle.h"
#include "../../inc/mh_args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool program_error(mh_context_t *context, const char *message, mh_code_location_t from) {
    char loc[128];
    mh_code_location_to_string(loc, from);
    fprintf(stderr, "An error has occurred %s: %s\n", loc, message);
    mh_end(context);
    exit(1);
}

void tests_print(const mh_test_t *tests, size_t count) {
    mh_test_return_t result;
    size_t failed = 0;
    for (size_t i = 0; i < count; i++) {
        printf("[......] [%zu/%zu] Running the test `%s`...", i + 1, count, tests[i].name);
        fflush(stdout);
        mh_tests_check(&result, &tests[i], 1);
        if (!result.success) {
            char loc[128];
            mh_code_location_to_string(loc, result.location);
            printf("\r[FAILED] [%zu/%zu] The test `%s` has failed because \"%s\" %s.\n", i + 1, count, tests[i].name,
                   result.reason, loc);
            failed++;
            if (tests[i].required) {
                fprintf(stderr, "`%s` is a required test, stopping...\n", tests[i].name);
                exit(1);
            }
        } else {
            printf("\r[PASSED] [%zu/%zu] The test `%s` has passed.\n", i + 1, count, tests[i].name);
        }
    }
    printf("Finished testing and ");
    if (failed) {
        printf("%zu out of %zu tests passed.\n", count - failed, count);
    } else {
        printf("all tests passed.\n");
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
        printf("%d", mh_tests_check(NULL, tests.tests, tests.count));
    } else {
        tests_print(tests.tests, tests.count);
    }
}
