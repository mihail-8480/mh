#include "../../inc/mh_tests.h"
#include <stdio.h>
#include <stdlib.h>

void mh_tests_run(const mh_test_t* tests, size_t count) {
    size_t failed = 0;
    for(size_t i = 0; i < count; i++) {
        if (tests[i].func == NULL) {
            failed++;
            continue;
        }
        printf("[......] [%zu/%zu] Running the test `%s`...", i+1, count, tests[i].name);
        fflush(stdout);
        mh_test_return_t result = tests[i].func();
        if (!result.success) {
            char loc[128];
            mh_code_location_to_string(loc, result.location);
            printf("\r[FAILED] [%zu/%zu] The test `%s` has failed because \"%s\" %s.\n", i+1, count, tests[i].name, result.reason, loc);
            failed++;
            if (tests[i].required) {
                fprintf(stderr,"`%s` is a required test, stopping...\n", tests[i].name);
                exit(1);
            }
        } else {
            printf("\r[PASSED] [%zu/%zu] The test `%s` has passed.\n", i+1, count, tests[i].name);
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


bool mh_tests_check(mh_test_return_t* results, const mh_test_t* tests, size_t count) {
    size_t failed = 0;
    for(size_t i = 0; i < count; i++) {
        if (tests[i].func == NULL) {
            if (results != NULL) {
                results[i].success = false;
                results[i].reason = "The test function is NULL.";
                results[i].location = MH_LOCATION_ANY();
            }
            failed++;
            continue;
        }
        mh_test_return_t result = tests[i].func();
        if (results != NULL) {
            results[i] = result;
        }
        if (!result.success && tests[i].required) {
                return false;
        }
    }
    return !failed;
}
