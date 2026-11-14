typedef void (*array_function)(void);

extern array_function __init_array_start[];
extern array_function __init_array_end[];
extern array_function __fini_array_start[];
extern array_function __fini_array_end[];
extern array_function __ctors_priority_start[];
extern array_function __ctors_priority_end[];
extern array_function __ctors_start[];
extern array_function __ctors_end[];
extern array_function __dtors_priority_start[];
extern array_function __dtors_priority_end[];
extern array_function __dtors_start[];
extern array_function __dtors_end[];

int WinMain(void *instance, void *previous, unsigned short *command_line, int show);
int DllMain(void *instance, unsigned long reason, void *reserved);
void __grimace_run_atexit(void);

#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0

void (*grimace_run_exit_handlers)(void);

static void run_constructors(void) {
    for (array_function *entry = __ctors_priority_start; entry < __ctors_priority_end; entry++) {
        (*entry)();
    }
    for (array_function *entry = __init_array_start; entry < __init_array_end; entry++) {
        (*entry)();
    }
    for (array_function *entry = __ctors_end; entry > __ctors_start;) {
        (*--entry)();
    }
}

static void run_destructors(void) {
    if (grimace_run_exit_handlers) {
        grimace_run_exit_handlers();
    }
    for (array_function *entry = __dtors_start; entry < __dtors_end; entry++) {
        (*entry)();
    }
    for (array_function *entry = __dtors_priority_end; entry > __dtors_priority_start;) {
        (*--entry)();
    }
    for (array_function *entry = __fini_array_end; entry > __fini_array_start;) {
        (*--entry)();
    }
}

void __grimace_finalize(void) {
    run_destructors();
}

int __grimace_exe_start(void *instance, void *previous, unsigned short *command_line, int show) {
    run_constructors();
    int result = WinMain(instance, previous, command_line, show);
    __grimace_run_atexit();
    run_destructors();
    return result;
}

int __grimace_dll_start(void *instance, unsigned long reason, void *reserved) {
    static int constructed;
    if (reason == DLL_PROCESS_ATTACH && !constructed) {
        run_constructors();
        constructed = 1;
    }
    int result = DllMain(instance, reason, reserved);
    if (reason == DLL_PROCESS_DETACH && constructed) {
        run_destructors();
        constructed = 0;
    }
    return result;
}
