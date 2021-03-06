#include "../../../inc/mh_tests.h"

bool mh_tests_check(mh_test_return_t *results, const mh_test_t *tests, size_t count) {
    size_t failed = 0;
    for (size_t i = 0; i < count; i++) {
        if (tests[i].func == NULL) {
            if (results != NULL) {
                results[i].success = false;
                results[i].reason = "The test function is NULL.";
                results[i].location = MH_LOCATION_ANY();
                results[i].seconds = -1;
            }
            failed++;
            continue;
        }
        mh_stopwatch_t stopwatch = mh_stopwatch_start();
        mh_test_return_t result = tests[i].func();
        mh_stopwatch_stop(&stopwatch);
        if (results != NULL) {
            results[i] = result;
            results[i].seconds = mh_stopwatch_value(stopwatch);
        }
        if (!result.success && tests[i].required) {
            return false;
        }
    }
    return !failed;
}
