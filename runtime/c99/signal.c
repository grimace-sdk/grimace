typedef void (*handler_type)(int);

#define SIGNALS 32
#define DEFAULT_HANDLER ((handler_type)0)
#define IGNORE_HANDLER ((handler_type)1)
#define ERROR_HANDLER ((handler_type)-1)

__attribute__((noreturn)) void _Exit(int status);
int *_errno(void);

static handler_type handlers[SIGNALS];

static int valid(int number) {
    return number > 0 && number < SIGNALS;
}

handler_type signal(int number, handler_type handler) {
    if (!valid(number)) {
        *_errno() = 22;
        return ERROR_HANDLER;
    }
    handler_type previous = handlers[number];
    handlers[number] = handler;
    return previous;
}

int raise(int number) {
    if (!valid(number)) {
        return -1;
    }
    handler_type handler = handlers[number];
    if (handler == IGNORE_HANDLER) {
        return 0;
    }
    if (handler == DEFAULT_HANDLER) {
        _Exit(3);
    }
    handlers[number] = DEFAULT_HANDLER;
    handler(number);
    return 0;
}
