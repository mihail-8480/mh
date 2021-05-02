#ifndef MH_TESTS_H
#define MH_TESTS_H
#include "mh.h"

MH_API_TYPE(mh_test_return, struct mh_test_return {
    bool success;
    const char* reason;
    mh_code_location_t location;
});

typedef mh_test_return_t (*mh_test_function_t)(void);

MH_API_TYPE(mh_test, struct mh_test {
    const char *name;
    const mh_test_function_t func;
    const bool required;
});


#define MH_TEST_NEW(f) mh_test_return_t f(void)

#define MH_TEST_FAIL() return (mh_test_return_t){.success = false, .reason = "No reason specified.", .location = MH_LOCATION_ANY()}
#define MH_TEST_PASSED() return (mh_test_return_t){.success = true, .reason = "Success.", .location = MH_LOCATION_ANY()}
#define MH_TEST_RESULT(value, r) return (mh_test_return_t){.success = value, .reason = r, .location = MH_LOCATION_ANY()}
#define MH_TEST(f) {.name = #f, .func = f, .required = false}
#define MH_TEST_REQUIRED(f) {.name = #f, .func = f, .required = true}

#define MH_TEST_EXPECT(cond) if (!(cond)) MH_TEST_RESULT(false, "The statement `" #cond "` is false.")

MH_API_FUNC(void mh_tests_run(const mh_test_t* tests, size_t count));

#endif //MH_TESTS_H
