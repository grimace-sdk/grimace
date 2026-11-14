#include <windows.h>

#include "runtime.h"

using array_function = void (*)();

extern "C" {
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
}

void (*grimace_run_exit_handlers)();

static void run_constructors() {
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

static void run_destructors() {
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

extern "C" void __grimace_run_atexit();

extern "C" void __grimace_cxx_finalize() {
    run_destructors();
}

extern "C" int WINAPI __grimace_exe_start(HINSTANCE instance, HINSTANCE previous, LPWSTR command_line, int show) {
    run_constructors();
    int result = WinMain(instance, previous, command_line, show);
    __grimace_run_atexit();
    run_destructors();
    return result;
}

extern "C" BOOL WINAPI __grimace_dll_start(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    static bool constructed;
    if (reason == DLL_PROCESS_ATTACH && !constructed) {
        run_constructors();
        constructed = true;
    }
    BOOL result = DllMain(instance, reason, reserved);
    if (reason == DLL_PROCESS_DETACH && constructed) {
        run_destructors();
        constructed = false;
    }
    return result;
}
