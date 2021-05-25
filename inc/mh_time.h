#ifndef MH_MH_TIME_H
#define MH_MH_TIME_H
#include "mh.h"
#include "time.h"

/*
 * <mh_time.h>
 * The libmh time header.
 *
 * Contains functions for keeping track of time.
 */

// Something that represents a point in time.
MH_API_TYPE(mh_time, struct timespec);

// A stopwatch - for measuring time.
MH_API_TYPE(mh_stopwatch, struct mh_stopwatch {
    // The time when the stopwatch started.
    mh_time_t start;
    // The time when the stopwatch stopped.
    mh_time_t end;
});

// How many seconds did an action take?
MH_API_TYPE(mh_seconds, long double);

// Start measuring time with a stopwatch.
MH_API_FUNC(mh_stopwatch_t mh_stopwatch_start(void));

// Stop measuring time with a stopwatch.
MH_API_FUNC(void mh_stopwatch_stop(mh_stopwatch_t *stopwatch));

// Reset and start a stopwatch.
MH_API_FUNC(void mh_stopwatch_restart(mh_stopwatch_t *stopwatch));

// Get the value of the stopwatch in seconds.
MH_API_FUNC(mh_seconds_t mh_stopwatch_value(mh_stopwatch_t stopwatch));

// A mh_time_t value that is initialized to zero.
#define MH_TIME_NULL (mh_time_t){0,0}

// Convert seconds to seconds.
#define MH_SECONDS (1)

// Convert seconds to milliseconds.
#define MH_MILLISECONDS (MH_SECONDS * 1000)

// Convert seconds to microseconds.
#define MH_MICROSECONDS (MH_MILLISECONDS * 1000)

// Convert seconds to nanoseconds.
#define MH_NANOSECONDS (MH_MICROSECONDS * 1000)

#endif //MH_MH_TIME_H
