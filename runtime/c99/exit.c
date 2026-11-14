#define HANDLERS 32

int _flushall(void);
__attribute__((noreturn)) void _Exit(int status);
extern void __grimace_cxx_finalize(void) __attribute__((weak));

static void (*handlers[HANDLERS])(void);
static int handler_count;

int atexit(void (*handler)(void)) {
    if (handler_count == HANDLERS) {
        return -1;
    }
    handlers[handler_count++] = handler;
    return 0;
}

void __grimace_run_atexit(void) {
    while (handler_count) {
        handlers[--handler_count]();
    }
}

__attribute__((noreturn)) void exit(int status) {
    __grimace_run_atexit();
    if (__grimace_cxx_finalize) {
        __grimace_cxx_finalize();
    }
    _flushall();
    _Exit(status);
}
