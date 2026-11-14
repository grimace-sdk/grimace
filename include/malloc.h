#ifndef GRIMACE_MALLOC_H
#define GRIMACE_MALLOC_H

#include <stdlib.h>
#include <alloca.h>

#define _alloca(size) __builtin_alloca(size)

#endif
