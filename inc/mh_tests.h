#ifndef MH_TESTS_H
#define MH_TESTS_H

#include "mh.h"

/*
 * <mh_tests.h>
 * The libmh testing header.
 *
 * It provides an unit testing system.
 */

// The return type of a test function.
MH_API_TYPE(mh_test_return, struct mh_test_return {
    // Did the test succeed?
    bool success;
    // If not, what was the reason it failed?
    mh_const_string_t reason;
    // The location in code where it failed.
    mh_code_location_t location;
});

// A test function pointer.
typedef mh_test_return_t (*mh_test_function_t)(void);

// A test.
MH_API_TYPE(mh_test, struct mh_test {
    // The name of the test.
    mh_const_string_t name;

    // A pointer to the test function.
    const mh_test_function_t func;

    // Is this test required? If true - it'll make the testing software instantly stop if this test fails.
    const bool required;
});


// An array of tests.
MH_API_TYPE(mh_tests, struct mh_tests {
    // The pointer to the first element of the test array.
    const mh_test_t *tests;

    // The number of tests in the array.
    size_t count;
});

// Create a new test.
#define MH_TEST_NEW(f) mh_test_return_t f(void)

// Make a test fail without a reason.
#define MH_TEST_FAIL() return (mh_test_return_t){.success = false, .reason = "No reason specified.", .location = MH_LOCATION_ANY()}

// Make the test pass.
#define MH_TEST_PASSED() return (mh_test_return_t){.success = true, .reason = "Success.", .location = MH_LOCATION_ANY()}

// Make the test return a specific result.
#define MH_TEST_RESULT(value, r) return (mh_test_return_t){.success = value, .reason = r, .location = MH_LOCATION_ANY()}

// Create a test structure based on a test function.
#define MH_TEST(f) {.name = #f, .func = f, .required = false}

// Create a required test structure based on a test function.
#define MH_TEST_REQUIRED(f) {.name = #f, .func = f, .required = true}

// Expect a condition to be true - fail the test otherwise.
#define MH_TEST_EXPECT(cond) if (!(cond)) MH_TEST_RESULT(false, "The statement `" #cond "` is false.")

// Check some tests, and put the results in results - if it's NULL will just check and will not attempt to store the result.
MH_API_FUNC(bool mh_tests_check(mh_test_return_t *results, const mh_test_t *tests, size_t count));

#endif //MH_TESTS_H
