#include "internal.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static WCHAR current_folder[MAX_PATH];

int __grimace_posix_fail(int number) {
    errno = number;
    return -1;
}

int __grimace_posix_fail_with_last_error(void) {
    switch (GetLastError()) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_DRIVE:
    case ERROR_NO_MORE_FILES:
        return __grimace_posix_fail(ENOENT);
    case ERROR_ACCESS_DENIED:
    case ERROR_SHARING_VIOLATION:
    case ERROR_LOCK_VIOLATION:
        return __grimace_posix_fail(EACCES);
    case ERROR_INVALID_HANDLE:
        return __grimace_posix_fail(EBADF);
    case ERROR_NOT_ENOUGH_MEMORY:
    case ERROR_OUTOFMEMORY:
        return __grimace_posix_fail(ENOMEM);
    case ERROR_FILE_EXISTS:
    case ERROR_ALREADY_EXISTS:
        return __grimace_posix_fail(EEXIST);
    case ERROR_DISK_FULL:
    case ERROR_HANDLE_DISK_FULL:
        return __grimace_posix_fail(ENOSPC);
    case ERROR_DIR_NOT_EMPTY:
        return __grimace_posix_fail(ENOTEMPTY);
    case ERROR_WRITE_PROTECT:
        return __grimace_posix_fail(EROFS);
    case ERROR_INVALID_NAME:
    case ERROR_INVALID_PARAMETER:
        return __grimace_posix_fail(EINVAL);
    case ERROR_FILENAME_EXCED_RANGE:
        return __grimace_posix_fail(ENAMETOOLONG);
    case ERROR_TOO_MANY_OPEN_FILES:
        return __grimace_posix_fail(EMFILE);
    default:
        return __grimace_posix_fail(EIO);
    }
}

static const WCHAR *folder(void) {
    if (!current_folder[0] && !__grimace_posix_module_folder(current_folder, MAX_PATH)) {
        current_folder[0] = '\\';
        current_folder[1] = 0;
    }
    return current_folder;
}

static int append(WCHAR *output, size_t *length, const WCHAR *text, size_t count) {
    if (*length + count >= MAX_PATH) {
        return 0;
    }
    for (size_t index = 0; index < count; index++) {
        output[(*length)++] = text[index];
    }
    output[*length] = 0;
    return 1;
}

static int add_segments(WCHAR *output, size_t *length, const WCHAR *path) {
    while (*path) {
        while (*path == '\\') {
            path++;
        }
        const WCHAR *end = path;
        while (*end && *end != '\\') {
            end++;
        }
        size_t count = end - path;
        if (count == 2 && path[0] == '.' && path[1] == '.') {
            while (*length > 0 && output[*length - 1] != '\\') {
                (*length)--;
            }
            if (*length > 0) {
                (*length)--;
            }
            output[*length] = 0;
        } else if (count > 0 && !(count == 1 && path[0] == '.')) {
            static const WCHAR separator = '\\';
            if (!append(output, length, &separator, 1) || !append(output, length, path, count)) {
                return 0;
            }
        }
        path = end;
    }
    return 1;
}

int __grimace_posix_resolve(const char *path, WCHAR *output) {
    WCHAR wide[MAX_PATH + 1];
    if (!path) {
        return __grimace_posix_fail(EFAULT);
    }
    if (!*path) {
        return __grimace_posix_fail(ENOENT);
    }
    size_t converted = mbstowcs(wide, path, MAX_PATH);
    if (converted == (size_t)-1) {
        return __grimace_posix_fail(EILSEQ);
    }
    if (converted >= MAX_PATH) {
        return __grimace_posix_fail(ENAMETOOLONG);
    }
    wide[converted] = 0;
    for (WCHAR *character = wide; *character; character++) {
        if (*character == '/') {
            *character = '\\';
        }
    }
    size_t length = 0;
    output[0] = 0;
    if (wide[0] != '\\' && !add_segments(output, &length, folder())) {
        return __grimace_posix_fail(ENAMETOOLONG);
    }
    if (!add_segments(output, &length, wide)) {
        return __grimace_posix_fail(ENAMETOOLONG);
    }
    if (length == 0) {
        output[0] = '\\';
        output[1] = 0;
    }
    return 0;
}

int chdir(const char *path) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    if (!(wide[0] == '\\' && wide[1] == 0)) {
        DWORD attributes = GetFileAttributesW(wide);
        if (attributes == 0xFFFFFFFF) {
            return __grimace_posix_fail_with_last_error();
        }
        if (!(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
            return __grimace_posix_fail(ENOTDIR);
        }
    }
    wcscpy(current_folder, wide);
    return 0;
}

char *getcwd(char *buffer, size_t size) {
    char narrow[MAX_PATH * 2 + 1];
    size_t length = wcstombs(narrow, folder(), sizeof narrow - 1);
    if (length == (size_t)-1) {
        __grimace_posix_fail(EILSEQ);
        return NULL;
    }
    narrow[length] = 0;
    if (!buffer) {
        size = size > length ? size : length + 1;
        buffer = malloc(size);
        if (!buffer) {
            __grimace_posix_fail(ENOMEM);
            return NULL;
        }
    } else if (size == 0) {
        __grimace_posix_fail(EINVAL);
        return NULL;
    }
    if (length + 1 > size) {
        __grimace_posix_fail(ERANGE);
        return NULL;
    }
    memcpy(buffer, narrow, length + 1);
    return buffer;
}

static int wide_mode(const char *mode, WCHAR *output) {
    size_t length = 0;
    while (mode[length] && length < 7) {
        output[length] = (unsigned char)mode[length];
        length++;
    }
    output[length] = 0;
    return length > 0;
}

FILE *__grimace_posix_fopen(const char *path, const char *mode) {
    WCHAR wide[MAX_PATH + 1];
    WCHAR wide_mode_text[8];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return NULL;
    }
    if (!wide_mode(mode, wide_mode_text)) {
        __grimace_posix_fail(EINVAL);
        return NULL;
    }
    FILE *file = _wfopen(wide, wide_mode_text);
    if (!file) {
        __grimace_posix_fail_with_last_error();
    }
    return file;
}

FILE *__grimace_posix_freopen(const char *path, const char *mode, FILE *stream) {
    WCHAR wide[MAX_PATH + 1];
    WCHAR wide_mode_text[8];
    if (!path) {
        __grimace_posix_fail(EINVAL);
        return NULL;
    }
    if (__grimace_posix_resolve(path, wide) < 0) {
        return NULL;
    }
    if (!wide_mode(mode, wide_mode_text)) {
        __grimace_posix_fail(EINVAL);
        return NULL;
    }
    FILE *file = _wfreopen(wide, wide_mode_text, stream);
    if (!file) {
        __grimace_posix_fail_with_last_error();
    }
    return file;
}

int __grimace_posix_remove(const char *path) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    DWORD attributes = GetFileAttributesW(wide);
    if (attributes != 0xFFFFFFFF && (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        return RemoveDirectoryW(wide) ? 0 : __grimace_posix_fail_with_last_error();
    }
    return DeleteFileW(wide) ? 0 : __grimace_posix_fail_with_last_error();
}

int __grimace_posix_rename(const char *existing, const char *replacement) {
    WCHAR wide_existing[MAX_PATH + 1];
    WCHAR wide_replacement[MAX_PATH + 1];
    if (__grimace_posix_resolve(existing, wide_existing) < 0 || __grimace_posix_resolve(replacement, wide_replacement) < 0) {
        return -1;
    }
    if (MoveFileW(wide_existing, wide_replacement)) {
        return 0;
    }
    if (GetLastError() != ERROR_ALREADY_EXISTS && GetLastError() != ERROR_FILE_EXISTS) {
        return __grimace_posix_fail_with_last_error();
    }
    DWORD attributes = GetFileAttributesW(wide_replacement);
    if (attributes != 0xFFFFFFFF && (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        return __grimace_posix_fail(EISDIR);
    }
    if (!DeleteFileW(wide_replacement) || !MoveFileW(wide_existing, wide_replacement)) {
        return __grimace_posix_fail_with_last_error();
    }
    return 0;
}
