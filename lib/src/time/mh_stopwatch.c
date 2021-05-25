#include "../../../inc/mh_time.h"

mh_stopwatch_t mh_stopwatch_start(void) {
    mh_stopwatch_t stopwatch;
    stopwatch.end = MH_TIME_NULL;
    mh_stopwatch_restart(&stopwatch);
    return stopwatch;
}

void mh_stopwatch_stop(mh_stopwatch_t *stopwatch) {
    clock_gettime(CLOCK_MONOTONIC, &stopwatch->end);
}

void mh_stopwatch_restart(mh_stopwatch_t *stopwatch) {
    clock_gettime(CLOCK_MONOTONIC, &stopwatch->start);
}

mh_seconds_t mh_stopwatch_value(mh_stopwatch_t stopwatch) {
    return (((double)stopwatch.end.tv_sec + 1.0e-9*(mh_seconds_t)stopwatch.end.tv_nsec) -
           (stopwatch.start.tv_sec + 1.0e-9*(mh_seconds_t)stopwatch.start.tv_nsec));
}
