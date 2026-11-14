#ifndef GRIMACE_POSIX_INTERNAL_H
#define GRIMACE_POSIX_INTERNAL_H

#include <windows.h>

int __grimace_posix_fail(int number);
int __grimace_posix_fail_with_last_error(void);
int __grimace_posix_resolve(const char *path, WCHAR *output);
int __grimace_posix_module_folder(WCHAR *output, unsigned long capacity);

#endif
