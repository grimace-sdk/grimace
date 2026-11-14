#ifndef GRIMACE_POSIX_SYS_TIME_H
#define GRIMACE_POSIX_SYS_TIME_H

#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

struct timeval {
    time_t tv_sec;
    long tv_usec;
};

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

int gettimeofday(struct timeval *time, void *zone);

#ifdef __cplusplus
}
#endif

#endif
