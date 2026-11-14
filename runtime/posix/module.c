extern unsigned long GetModuleFileNameW(void *module, unsigned short *path, unsigned long capacity) __attribute__((weak));

int __grimace_posix_module_folder(unsigned short *output, unsigned long capacity) {
    unsigned long length = GetModuleFileNameW ? GetModuleFileNameW(0, output, capacity) : 0;
    if (length == 0 || length >= capacity) {
        return 0;
    }
    while (length > 0 && output[length - 1] != '\\') {
        length--;
    }
    if (length > 1) {
        length--;
    }
    output[length] = 0;
    return length > 0;
}
