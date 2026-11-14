#ifndef GRIMACE_POSIX_STDIO_H
#define GRIMACE_POSIX_STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

FILE *fopen(const char *path, const char *mode) __asm__("__grimace_posix_fopen");
FILE *freopen(const char *path, const char *mode, FILE *stream) __asm__("__grimace_posix_freopen");
int remove(const char *path) __asm__("__grimace_posix_remove");
int rename(const char *existing, const char *replacement) __asm__("__grimace_posix_rename");
#undef fileno
int fileno(FILE *stream);
FILE *fdopen(int descriptor, const char *mode);
#ifndef GRIMACE_IOSTREAMS
int fseeko(FILE *stream, long offset, int origin);
long ftello(FILE *stream);
#endif

#ifdef __cplusplus
}
#endif

#endif
