#include "internal.h"
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

struct grimace_directory {
    WCHAR pattern[MAX_PATH + 3];
    HANDLE search;
    WIN32_FIND_DATAW found;
    int pending;
    struct dirent entry;
};

static void start(DIR *directory) {
    directory->search = FindFirstFileW(directory->pattern, &directory->found);
    directory->pending = directory->search != INVALID_HANDLE_VALUE;
}

DIR *opendir(const char *path) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return NULL;
    }
    if (!(wide[0] == '\\' && wide[1] == 0)) {
        DWORD attributes = GetFileAttributesW(wide);
        if (attributes == 0xFFFFFFFF) {
            __grimace_posix_fail_with_last_error();
            return NULL;
        }
        if (!(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
            __grimace_posix_fail(ENOTDIR);
            return NULL;
        }
    }
    DIR *directory = calloc(1, sizeof *directory);
    if (!directory) {
        __grimace_posix_fail(ENOMEM);
        return NULL;
    }
    size_t length = wcslen(wide);
    wcscpy(directory->pattern, wide);
    if (length == 1) {
        length = 0;
    }
    directory->pattern[length] = '\\';
    directory->pattern[length + 1] = '*';
    directory->pattern[length + 2] = 0;
    start(directory);
    return directory;
}

struct dirent *readdir(DIR *directory) {
    while (directory->pending) {
        directory->pending = 0;
        size_t length = wcstombs(directory->entry.d_name, directory->found.cFileName, sizeof directory->entry.d_name - 1);
        int usable = length != (size_t)-1;
        if (usable) {
            directory->entry.d_name[length] = 0;
            directory->entry.d_type = (directory->found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? DT_DIR : DT_REG;
            directory->entry.d_ino = 0;
        }
        directory->pending = FindNextFileW(directory->search, &directory->found);
        if (usable) {
            return &directory->entry;
        }
    }
    return NULL;
}

void rewinddir(DIR *directory) {
    if (directory->search != INVALID_HANDLE_VALUE) {
        FindClose(directory->search);
    }
    start(directory);
}

int closedir(DIR *directory) {
    if (!directory) {
        return __grimace_posix_fail(EBADF);
    }
    if (directory->search != INVALID_HANDLE_VALUE) {
        FindClose(directory->search);
    }
    free(directory);
    return 0;
}
