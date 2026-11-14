#include "internal.h"
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int gettimeofday(struct timeval *time, void *zone) {
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    time->tv_sec = now.tv_sec;
    time->tv_usec = now.tv_nsec / 1000;
    if (zone) {
        struct timezone *offset = zone;
        offset->tz_minuteswest = 0;
        offset->tz_dsttime = 0;
    }
    return 0;
}

int clock_gettime(clockid_t clock, struct timespec *time) {
    if (clock == CLOCK_REALTIME) {
        timespec_get(time, TIME_UTC);
    } else if (clock == CLOCK_MONOTONIC) {
        timespec_get(time, TIME_MONOTONIC);
    } else {
        return __grimace_posix_fail(EINVAL);
    }
    return 0;
}

int clock_getres(clockid_t clock, struct timespec *resolution) {
    if (clock != CLOCK_REALTIME && clock != CLOCK_MONOTONIC) {
        return __grimace_posix_fail(EINVAL);
    }
    if (resolution) {
        resolution->tv_sec = 0;
        resolution->tv_nsec = 1000000;
    }
    return 0;
}

int usleep(unsigned int microseconds) {
    Sleep((microseconds + 999) / 1000);
    return 0;
}

unsigned int sleep(unsigned int seconds) {
    Sleep(seconds * 1000);
    return 0;
}

int nanosleep(const struct timespec *duration, struct timespec *remaining) {
    if (duration->tv_nsec < 0 || duration->tv_nsec >= 1000000000) {
        return __grimace_posix_fail(EINVAL);
    }
    Sleep((DWORD)duration->tv_sec * 1000 + (DWORD)((duration->tv_nsec + 999999) / 1000000));
    if (remaining) {
        remaining->tv_sec = 0;
        remaining->tv_nsec = 0;
    }
    return 0;
}
