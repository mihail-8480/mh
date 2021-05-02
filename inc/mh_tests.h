#ifndef MH_TESTS_H
#define MH_TESTS_H
#include "mh.h"

MH_API_TYPE(mh_test_return, bool);

typedef mh_test_return_t (*mh_test_function_t)(void);

MH_API_TYPE(mh_test, struct mh_test {
    const char *name;
    const mh_test_function_t func;
    const bool required;
});


#define MH_TEST_NEW(f) mh_test_return_t f(void)

#define MH_TEST_FAIL() return false
#define MH_TEST_PASSED() return true
#define MH_TEST_RESULT(value) return value
#define MH_TEST(f) {.name = #f, .func = f, .required = false}
#define MH_REQUIRED_TEST(f) {.name = #f, .func = f, .required = true}

MH_API_FUNC(void mh_tests_run(const mh_test_t* tests, size_t count));

#endif //MH_TESTS_H
