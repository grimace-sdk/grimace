#include <stddef.h>

typedef struct grimace_file FILE;

int setvbuf(FILE *stream, char *buffer, int mode, size_t size) {
    return mode == 0x0000 || mode == 0x0040 || mode == 0x0004 ? 0 : -1;
}
