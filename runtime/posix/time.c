#include "internal.h"
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define UNIX_EPOCH_IN_FILETIME 116444736000000000ULL

static unsigned long long clock_microseconds(void) {
    SYSTEMTIME now;
    FILETIME converted;
    GetSystemTime(&now);
    if (!SystemTimeToFileTime(&now, &converted)) {
        return 0;
    }
    unsigned long long ticks = ((unsigned long long)converted.dwHighDateTime << 32) | converted.dwLowDateTime;
    return (ticks - UNIX_EPOCH_IN_FILETIME) / 10;
}

static unsigned long long anchor_microseconds;
static DWORD anchor_tick;

int gettimeofday(struct timeval *time, void *zone) {
    unsigned long long clock_now = clock_microseconds();
    DWORD tick = GetTickCount();
    unsigned long long now = anchor_microseconds + (unsigned long long)(DWORD)(tick - anchor_tick) * 1000;
    long long drift = (long long)(now - clock_now);
    if (!anchor_microseconds || drift > 2000000 || drift < -2000000) {
        anchor_microseconds = clock_now;
        anchor_tick = tick;
        now = clock_now;
    }
    time->tv_sec = (time_t)(now / 1000000);
    time->tv_usec = (long)(now % 1000000);
    if (zone) {
        struct timezone *offset = zone;
        offset->tz_minuteswest = 0;
        offset->tz_dsttime = 0;
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
