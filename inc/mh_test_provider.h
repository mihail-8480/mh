#ifndef MH_MH_TEST_PROVIDER_H
#define MH_MH_TEST_PROVIDER_H

#include "mh_tests.h"

/*
 * <mh_test_provider.h>
 * The libmh test provider header.
 *
 * Every library that provides tests should include this and provide an implementation for mh_test_provider().
 */

mh_tests_t mh_test_provider(void);

#endif //MH_MH_TEST_PROVIDER_H
