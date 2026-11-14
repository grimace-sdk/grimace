#ifndef GRIMACE_TIME_H
#define GRIMACE_TIME_H

#include <grimace/version.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _TIME_T_DEFINED
#if GRIMACE_CE >= 3
typedef unsigned long time_t;
#else
typedef long time_t;
#endif
#define _TIME_T_DEFINED
#endif

/**
 * Returns the difference between two times in seconds.
 *
 * @param end The later time.
 * @param start The earlier time.
 * @return end minus start, in seconds.
 *
 * @note Windows CE 2.0 and later.
 */
double difftime(time_t end, time_t start);

#ifndef _CLOCK_T_DEFINED
typedef long clock_t;
#define _CLOCK_T_DEFINED
#endif

#define CLOCKS_PER_SEC 1000

#include <grimace/tm.h>

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

time_t time(time_t *result);
clock_t clock(void);
struct tm *gmtime(const time_t *time);
struct tm *localtime(const time_t *time);
time_t mktime(struct tm *time);
char *asctime(const struct tm *time);
char *ctime(const time_t *time);
size_t strftime(char *buffer, size_t size, const char *format, const struct tm *time);

#ifdef GRIMACE_POSIX
struct tm *gmtime_r(const time_t *time, struct tm *result);
struct tm *localtime_r(const time_t *time, struct tm *result);
int nanosleep(const struct timespec *duration, struct timespec *remaining);
#endif

#ifdef __cplusplus
}
#endif

#endif
