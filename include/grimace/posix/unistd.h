#ifndef GRIMACE_POSIX_UNISTD_H
#define GRIMACE_POSIX_UNISTD_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4

ssize_t read(int descriptor, void *buffer, size_t count);
ssize_t write(int descriptor, const void *buffer, size_t count);
off_t lseek(int descriptor, off_t offset, int whence);
int close(int descriptor);
int fsync(int descriptor);
int ftruncate(int descriptor, off_t length);
int truncate(const char *path, off_t length);
int dup(int descriptor);
pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);
int isatty(int descriptor);
int unlink(const char *path);
int rmdir(const char *path);
int access(const char *path, int mode);
int chdir(const char *path);
char *getcwd(char *buffer, size_t size);
int usleep(unsigned int microseconds);
unsigned int sleep(unsigned int seconds);

#ifdef __cplusplus
}
#endif

#endif
