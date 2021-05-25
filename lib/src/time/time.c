#include "../../../inc/mh_time.h"

mh_time_t mh_clock_now(void) {
    mh_time_t time = MH_TIME_NULL;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time;
}

mh_seconds_t mh_time_to_seconds(mh_time_t time) {
    return time.tv_sec + (1.0/MH_NANOSECONDS) * (mh_seconds_t)time.tv_nsec;
}
