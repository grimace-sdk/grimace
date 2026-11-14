#include <stddef.h>

typedef struct grimace_file FILE;

FILE *_getstdfilex(int index);
int fputs(const char *string, FILE *stream);
int fseek(FILE *stream, long offset, int origin);
void clearerr(FILE *stream);
char *strerror(int number);
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
