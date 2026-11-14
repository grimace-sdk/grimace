#ifndef GRIMACE_POSIX_FCNTL_H
#define GRIMACE_POSIX_FCNTL_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define O_RDONLY 0x0000
#define O_WRONLY 0x0001
#define O_RDWR 0x0002
#define O_ACCMODE 0x0003
#define O_APPEND 0x0008
#define O_CREAT 0x0100
#define O_TRUNC 0x0200
#define O_EXCL 0x0400
#define O_TEXT 0x4000
#define O_BINARY 0x8000

int open(const char *path, int flags, ...);
int creat(const char *path, mode_t mode);

#ifdef __cplusplus
}
#endif

#endif
