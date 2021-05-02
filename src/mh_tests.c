#include "../inc/mh_tests.h"
#include <stdio.h>
#include <stdlib.h>

MH_NORETURN void mh_tests_run(const mh_test_t* tests, size_t count) {
    size_t failed = 0;
    for(size_t i = 0; i < count; i++) {
        if (tests[i].func == NULL) {
            failed++;
            continue;
        }
        printf("[......] [%zu/%zu] Running test `%s`...", i+1, count, tests[i].name);
        fflush(stdout);
        mh_test_return_t result = tests[i].func();
        if (!result.success) {
            char loc[128];
            mh_code_location_to_string(loc, result.location);
            printf("\r[FAILED] [%zu/%zu] Failed test `%s` because \"%s\" %s.\n", i+1, count, tests[i].name, result.reason, loc);
            failed++;
            if (tests[i].required) {
                fprintf(stderr,"%s is a required test, stopping...\n", tests[i].name);
                exit(1);
            }
        } else {
            printf("\r[PASSED] [%zu/%zu] Passed test `%s`.\n", i+1, count, tests[i].name);
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
