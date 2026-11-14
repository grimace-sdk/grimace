#ifndef GRIMACE_POSIX_SYS_PARAM_H
#define GRIMACE_POSIX_SYS_PARAM_H

#include <limits.h>
#include <sys/types.h>

#define MAXPATHLEN 260

#ifndef MIN
#define MIN(first, second) ((first) < (second) ? (first) : (second))
#endif
#ifndef MAX
#define MAX(first, second) ((first) > (second) ? (first) : (second))
#endif

#endif
