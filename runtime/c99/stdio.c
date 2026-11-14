#include <stddef.h>

typedef struct grimace_file FILE;

FILE *_getstdfilex(int index);
int fputs(const char *string, FILE *stream);
int fseek(FILE *stream, long offset, int origin);
void clearerr(FILE *stream);
char *strerror(int number);
int fflush(FILE *stream);
FILE *_wfreopen(const unsigned short *path, const unsigned short *mode, FILE *stream);
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);
int *_errno(void);

void perror(const char *prefix) {
    FILE *error = _getstdfilex(2);
    if (prefix && *prefix) {
        fputs(prefix, error);
        fputs(": ", error);
    }
    fputs(strerror(*_errno()), error);
    fputs("\n", error);
}

void rewind(FILE *stream) {
    fseek(stream, 0, 0);
    clearerr(stream);
}

__attribute__((noreturn)) void abort(void);
int _snprintf(char *buffer, size_t count, const char *format, ...);

__attribute__((noreturn)) void __grimace_assert(const char *expression, const char *file, int line, const char *function) {
    char message[256];
    _snprintf(message, sizeof message - 1, "Assertion failed: %s, %s, %s line %d\n", expression, function, file, line);
    message[sizeof message - 1] = 0;
    fputs(message, _getstdfilex(2));
    abort();
}

#define UNBUFFERED 4

static int redirected;

int __grimace_stdio_redirected(void) {
    return redirected;
}

static int reopen(FILE *stream, const unsigned short *path) {
    static const unsigned short append[] = {'a', 0};
    if (!_wfreopen(path, append, stream)) {
        return 0;
    }
    setvbuf(stream, NULL, UNBUFFERED, 0);
    return 1;
}

int grimace_redirect_stdio(const unsigned short *path) {
    FILE *output = _getstdfilex(1);
    FILE *error = _getstdfilex(2);
    fflush(output);
    fflush(error);
    if (!reopen(output, path)) {
        return -1;
    }
    redirected = 1;
    return reopen(error, path) ? 0 : 1;
}

void setbuf(FILE *stream, char *buffer) {
    setvbuf(stream, buffer, buffer ? 0x0000 : UNBUFFERED, 512);
}
