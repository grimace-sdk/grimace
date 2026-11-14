#define HANDLERS 32

int _flushall(void);
__attribute__((noreturn)) void _Exit(int status);
void __grimace_finalize(void);

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
    __grimace_finalize();
    _flushall();
    _Exit(status);
}
