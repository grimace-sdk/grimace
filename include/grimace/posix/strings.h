#ifndef GRIMACE_POSIX_STRINGS_H
#define GRIMACE_POSIX_STRINGS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int strcasecmp(const char *first, const char *second);
int strncasecmp(const char *first, const char *second, size_t count);

#ifdef __cplusplus
}
#endif

#endif
