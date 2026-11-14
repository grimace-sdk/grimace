#include "internal.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define DESCRIPTORS 64
#define FIRST_FILE 3
#define UNIX_EPOCH_IN_FILETIME 116444736000000000ULL

typedef struct {
    HANDLE handle;
    int flags;
    int borrowed;
} descriptor_entry;

static descriptor_entry descriptors[DESCRIPTORS];

static descriptor_entry *file_entry(int descriptor) {
    if (descriptor < FIRST_FILE || descriptor >= DESCRIPTORS || !descriptors[descriptor].handle) {
        return NULL;
    }
    return &descriptors[descriptor];
}

static time_t unix_time(const FILETIME *time) {
    unsigned long long ticks = ((unsigned long long)time->dwHighDateTime << 32) | time->dwLowDateTime;
    if (ticks < UNIX_EPOCH_IN_FILETIME) {
        return 0;
    }
    return (time_t)((ticks - UNIX_EPOCH_IN_FILETIME) / 10000000);
}

static void describe(struct stat *status, DWORD attributes, DWORD size, const FILETIME *created, const FILETIME *accessed,
                     const FILETIME *written) {
    memset(status, 0, sizeof *status);
    status->st_mode = (attributes & FILE_ATTRIBUTE_DIRECTORY) ? S_IFDIR | 0555 : S_IFREG | 0444;
    if (!(attributes & FILE_ATTRIBUTE_READONLY)) {
        status->st_mode |= 0200;
    }
    status->st_nlink = 1;
    status->st_size = (off_t)size;
    status->st_mtime = unix_time(written);
    status->st_atime = accessed->dwLowDateTime || accessed->dwHighDateTime ? unix_time(accessed) : status->st_mtime;
    status->st_ctime = created->dwLowDateTime || created->dwHighDateTime ? unix_time(created) : status->st_mtime;
}

int open(const char *path, int flags, ...) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    int descriptor = FIRST_FILE;
    while (descriptor < DESCRIPTORS && descriptors[descriptor].handle) {
        descriptor++;
    }
    if (descriptor == DESCRIPTORS) {
        return __grimace_posix_fail(EMFILE);
    }
    DWORD access;
    switch (flags & O_ACCMODE) {
    case O_RDONLY:
        access = GENERIC_READ;
        break;
    case O_WRONLY:
        access = GENERIC_WRITE;
        break;
    case O_RDWR:
        access = GENERIC_READ | GENERIC_WRITE;
        break;
    default:
        return __grimace_posix_fail(EINVAL);
    }
    DWORD disposition;
    if ((flags & O_CREAT) && (flags & O_EXCL)) {
        disposition = CREATE_NEW;
    } else if ((flags & O_CREAT) && (flags & O_TRUNC)) {
        disposition = CREATE_ALWAYS;
    } else if (flags & O_CREAT) {
        disposition = OPEN_ALWAYS;
    } else if (flags & O_TRUNC) {
        disposition = TRUNCATE_EXISTING;
    } else {
        disposition = OPEN_EXISTING;
    }
    DWORD attributes = GetFileAttributesW(wide);
    if (attributes != 0xFFFFFFFF && (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        return __grimace_posix_fail(EISDIR);
    }
    HANDLE handle = CreateFileW(wide, access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, disposition, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        return __grimace_posix_fail_with_last_error();
    }
    descriptors[descriptor].handle = handle;
    descriptors[descriptor].flags = flags;
    descriptors[descriptor].borrowed = 0;
    return descriptor;
}

int creat(const char *path, mode_t mode) {
    return open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

ssize_t read(int descriptor, void *buffer, size_t count) {
    if (descriptor == STDIN_FILENO) {
        return 0;
    }
    descriptor_entry *entry = file_entry(descriptor);
    if (!entry || (entry->flags & O_ACCMODE) == O_WRONLY) {
        return __grimace_posix_fail(EBADF);
    }
    DWORD transferred;
    if (!ReadFile(entry->handle, buffer, count, &transferred, NULL)) {
        return __grimace_posix_fail_with_last_error();
    }
    return (ssize_t)transferred;
}

static void write_debug_output(const unsigned char *bytes, size_t count) {
    WCHAR wide[129];
    while (count) {
        size_t length = count < 128 ? count : 128;
        for (size_t index = 0; index < length; index++) {
            wide[index] = bytes[index];
        }
        wide[length] = 0;
        OutputDebugStringW(wide);
        bytes += length;
        count -= length;
    }
}

ssize_t write(int descriptor, const void *buffer, size_t count) {
    if (descriptor == STDOUT_FILENO || descriptor == STDERR_FILENO) {
        write_debug_output(buffer, count);
        return (ssize_t)count;
    }
    descriptor_entry *entry = file_entry(descriptor);
    if (!entry || (entry->flags & O_ACCMODE) == O_RDONLY) {
        return __grimace_posix_fail(EBADF);
    }
    if (entry->flags & O_APPEND) {
        SetFilePointer(entry->handle, 0, NULL, FILE_END);
    }
    DWORD transferred;
    if (!WriteFile(entry->handle, buffer, count, &transferred, NULL)) {
        return __grimace_posix_fail_with_last_error();
    }
    return (ssize_t)transferred;
}

off_t lseek(int descriptor, off_t offset, int whence) {
    descriptor_entry *entry = file_entry(descriptor);
    if (!entry) {
        return __grimace_posix_fail(descriptor >= 0 && descriptor < FIRST_FILE ? ESPIPE : EBADF);
    }
    if (whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END) {
        return __grimace_posix_fail(EINVAL);
    }
    SetLastError(NO_ERROR);
    DWORD position = SetFilePointer(entry->handle, offset, NULL, whence);
    if (position == 0xFFFFFFFF && GetLastError() != NO_ERROR) {
        return __grimace_posix_fail_with_last_error();
    }
    return (off_t)position;
}

int close(int descriptor) {
    if (descriptor >= 0 && descriptor < FIRST_FILE) {
        return 0;
    }
    descriptor_entry *entry = file_entry(descriptor);
    if (!entry) {
        return __grimace_posix_fail(EBADF);
    }
    HANDLE handle = entry->handle;
    entry->handle = NULL;
    if (entry->borrowed) {
        return 0;
    }
    return CloseHandle(handle) ? 0 : __grimace_posix_fail_with_last_error();
}

int fsync(int descriptor) {
    if (descriptor >= 0 && descriptor < FIRST_FILE) {
        return 0;
    }
    descriptor_entry *entry = file_entry(descriptor);
    if (!entry) {
        return __grimace_posix_fail(EBADF);
    }
    return FlushFileBuffers(entry->handle) ? 0 : __grimace_posix_fail_with_last_error();
}

int ftruncate(int descriptor, off_t length) {
    descriptor_entry *entry = file_entry(descriptor);
    if (!entry || (entry->flags & O_ACCMODE) == O_RDONLY) {
        return __grimace_posix_fail(EBADF);
    }
    if (length < 0) {
        return __grimace_posix_fail(EINVAL);
    }
    DWORD position = SetFilePointer(entry->handle, 0, NULL, FILE_CURRENT);
    int truncated = SetFilePointer(entry->handle, length, NULL, FILE_BEGIN) != 0xFFFFFFFF && SetEndOfFile(entry->handle);
    int result = truncated ? 0 : __grimace_posix_fail_with_last_error();
    SetFilePointer(entry->handle, position, NULL, FILE_BEGIN);
    return result;
}

int isatty(int descriptor) {
    int open_descriptor = (descriptor >= 0 && descriptor < FIRST_FILE) || file_entry(descriptor);
    errno = open_descriptor ? ENOTTY : EBADF;
    return 0;
}

int unlink(const char *path) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    return DeleteFileW(wide) ? 0 : __grimace_posix_fail_with_last_error();
}

int rmdir(const char *path) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    return RemoveDirectoryW(wide) ? 0 : __grimace_posix_fail_with_last_error();
}

int mkdir(const char *path, mode_t mode) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    return CreateDirectoryW(wide, NULL) ? 0 : __grimace_posix_fail_with_last_error();
}

int access(const char *path, int mode) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    DWORD attributes = GetFileAttributesW(wide);
    if (attributes == 0xFFFFFFFF) {
        return __grimace_posix_fail_with_last_error();
    }
    if ((mode & W_OK) && (attributes & FILE_ATTRIBUTE_READONLY)) {
        return __grimace_posix_fail(EACCES);
    }
    return 0;
}

int stat(const char *path, struct stat *status) {
    WCHAR wide[MAX_PATH + 1];
    if (__grimace_posix_resolve(path, wide) < 0) {
        return -1;
    }
    if (wide[0] == '\\' && wide[1] == 0) {
        FILETIME none = {0, 0};
        describe(status, FILE_ATTRIBUTE_DIRECTORY, 0, &none, &none, &none);
        return 0;
    }
    WIN32_FIND_DATAW found;
    HANDLE search = FindFirstFileW(wide, &found);
    if (search == INVALID_HANDLE_VALUE) {
        return __grimace_posix_fail_with_last_error();
    }
    FindClose(search);
    describe(status, found.dwFileAttributes, found.nFileSizeLow, &found.ftCreationTime, &found.ftLastAccessTime, &found.ftLastWriteTime);
    return 0;
}

int fstat(int descriptor, struct stat *status) {
    if (descriptor >= 0 && descriptor < FIRST_FILE) {
        memset(status, 0, sizeof *status);
        status->st_mode = S_IFCHR | (descriptor == STDIN_FILENO ? 0444 : 0222);
        status->st_nlink = 1;
        return 0;
    }
    descriptor_entry *entry = file_entry(descriptor);
    if (!entry) {
        return __grimace_posix_fail(EBADF);
    }
    FILETIME created;
    FILETIME accessed;
    FILETIME written;
    DWORD size = GetFileSize(entry->handle, NULL);
    if (size == 0xFFFFFFFF || !GetFileTime(entry->handle, &created, &accessed, &written)) {
        return __grimace_posix_fail_with_last_error();
    }
    describe(status, FILE_ATTRIBUTE_NORMAL, size, &created, &accessed, &written);
    return 0;
}

int fileno(FILE *stream) {
    if (!stream) {
        return __grimace_posix_fail(EBADF);
    }
    for (int standard = 0; standard < FIRST_FILE; standard++) {
        if (stream == _getstdfilex(standard)) {
            return standard;
        }
    }
    HANDLE handle = _fileno(stream);
    if (!handle || handle == INVALID_HANDLE_VALUE) {
        return __grimace_posix_fail(EBADF);
    }
    int free_descriptor = -1;
    for (int descriptor = FIRST_FILE; descriptor < DESCRIPTORS; descriptor++) {
        if (descriptors[descriptor].handle == handle) {
            return descriptor;
        }
        if (!descriptors[descriptor].handle && free_descriptor < 0) {
            free_descriptor = descriptor;
        }
    }
    if (free_descriptor < 0) {
        return __grimace_posix_fail(EMFILE);
    }
    descriptors[free_descriptor].handle = handle;
    descriptors[free_descriptor].flags = O_RDWR;
    descriptors[free_descriptor].borrowed = 1;
    return free_descriptor;
}
