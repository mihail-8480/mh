#include "../../../inc/mh_time.h"

mh_stopwatch_t mh_stopwatch_start(void) {
    mh_stopwatch_t stopwatch;
    mh_stopwatch_restart(&stopwatch);
    return stopwatch;
}

void mh_stopwatch_stop(mh_stopwatch_t *stopwatch) {
    stopwatch->end = mh_clock_now();
}

void mh_stopwatch_restart(mh_stopwatch_t *stopwatch) {
    stopwatch->start = mh_clock_now();
}

mh_seconds_t mh_stopwatch_value(mh_stopwatch_t stopwatch) {
    return mh_time_to_seconds(stopwatch.end) - mh_time_to_seconds(stopwatch.start);
}
