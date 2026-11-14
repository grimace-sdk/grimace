#ifndef GRIMACE_POSIX_DIRENT_H
#define GRIMACE_POSIX_DIRENT_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DT_UNKNOWN 0
#define DT_DIR 4
#define DT_REG 8

struct dirent {
    ino_t d_ino;
    unsigned char d_type;
    char d_name[520];
};

typedef struct grimace_directory DIR;

DIR *opendir(const char *path);
struct dirent *readdir(DIR *directory);
void rewinddir(DIR *directory);
int closedir(DIR *directory);

#ifdef __cplusplus
}
#endif

#endif
