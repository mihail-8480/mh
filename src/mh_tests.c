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
        printf("[%zu/%zu] Running test `%s`...", i+1, count, tests[i].name);
        bool result = tests[i].func();
        if (!result) {
            printf(" [FAILED]\n");
            failed++;
            if (tests[i].required) {
                fprintf(stderr,"%s is a required test, stopping...\n", tests[i].name);
                exit(1);
            }
        } else {
            printf(" [PASSED]\n");
        }
    }
    printf("%zu/%zu tests passed.\n", count-failed, count);
    exit(failed != 0);
}
