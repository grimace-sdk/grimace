#include "internal.h"
#include <errno.h>
#include <pwd.h>
#include <time.h>
#include <utime.h>

#define UNIX_EPOCH_IN_FILETIME 116444736000000000ULL

static FILETIME file_time(time_t seconds) {
    unsigned long long ticks = (unsigned long long)seconds * 10000000 + UNIX_EPOCH_IN_FILETIME;
    FILETIME result = {(DWORD)ticks, (DWORD)(ticks >> 32)};
    return result;
}

int utime(const char *path, const struct utimbuf *times) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    time_t now = time(NULL);
    FILETIME accessed = file_time(times ? times->actime : now);
    FILETIME modified = file_time(times ? times->modtime : now);
    HANDLE handle = CreateFileW(wide, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        return __grimace_posix_fail_with_last_error();
    }
    int result = SetFileTime(handle, NULL, &accessed, &modified) ? 0 : __grimace_posix_fail_with_last_error();
    CloseHandle(handle);
    return result;
}

struct passwd *getpwuid(uid_t user) {
    return NULL;
}

struct passwd *getpwnam(const char *name) {
    return NULL;
}
