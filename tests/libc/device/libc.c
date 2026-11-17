#include <windows.h>
#include <alloca.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fenv.h>
#include <fcntl.h>
#include <inttypes.h>
#include <locale.h>
#include <memory.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <utime.h>
#include <pwd.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <wctype.h>

static HANDLE results = INVALID_HANDLE_VALUE;
static WCHAR summary[1024];
static int passed;
static int failed;

static void line(const WCHAR *text) {
    DWORD written;
    OutputDebugStringW(text);
    if (results != INVALID_HANDLE_VALUE) {
        WriteFile(results, text, wcslen(text) * sizeof(WCHAR), &written, NULL);
    }
}

static void check(const char *name, int ok, const char *detail) {
    WCHAR text[256];
    if (ok) {
        passed++;
        return;
    }
    failed++;
    _snwprintf(text, 255, L"libc: FAIL %hs: %hs\r\n", name, detail ? detail : "");
    text[255] = 0;
    line(text);
    if (wcslen(summary) + wcslen(text) < 1000) {
        wcscat(summary, text + 6);
    }
}

static void check_text(const char *name, const char *actual, const char *expected) {
    char detail[200];
    _snprintf(detail, sizeof detail - 1, "got [%s] expected [%s]", actual, expected);
    detail[sizeof detail - 1] = 0;
    check(name, !strcmp(actual, expected), detail);
}

static void check_close(const char *name, double actual, double expected) {
    char detail[100];
    _snprintf(detail, sizeof detail - 1, "got %.17g expected %.17g", actual, expected);
    detail[sizeof detail - 1] = 0;
    check(name, fabs(actual - expected) <= fabs(expected) * 1e-15, detail);
}

static int compare_ints(const void *first, const void *second) {
    return *(const int *)first - *(const int *)second;
}

static jmp_buf jump;

static void jump_back(int value) {
    longjmp(jump, value);
}

static void formatting(void) {
    char text[200];
    sprintf(text, "%d|%5d|%-5d|%05d|%x|%#X|%o|%u|%c|%s|%.2s|%%", -42, 42, 42, 42, 255, 255, 8, 4000000000u, 'z', "text", "text");
    check_text("sprintf integers", text, "-42|   42|42   |00042|ff|0XFF|10|4000000000|z|text|te|%");
    sprintf(text, "%f|%.3f|%e|%.2E|%g|%g|%g|%.0f", 3.14159265358979, 2.0006, 12345.678, 0.000123, 100000.0, 1000000.0, 0.0001, 2.5);
    check_text("sprintf floats", text, "3.141593|2.001|1.234568e+004|1.23E-004|100000|1e+006|0.0001|3");
    sprintf(text, "%I64d|%I64x|%S", -1234567890123ll, 0x123456789Aull, L"wide");
    check_text("sprintf I64 and %S", text, "-1234567890123|123456789a|wide");
    sprintf(text, "%" PRId64 "|%" PRIX64 "|%" PRIu32, (int64_t)-1234567890123ll, (uint64_t)0x123456789Aull, (uint32_t)4000000000u);
    check_text("sprintf inttypes", text, "-1234567890123|123456789A|4000000000");
    check("_snprintf truncates", _snprintf(text, 4, "%d", 123456) == -1 && !strncmp(text, "1234", 4), NULL);
    check("snprintf C99", snprintf(text, 4, "%d", 123456) == 6 && !strcmp(text, "123"), NULL);
    WCHAR wide[64];
    swprintf(wide, L"%s %d %.1f %hs", L"wide", 7, 0.25, "narrow");
    check("swprintf", !wcscmp(wide, L"wide 7 0.3 narrow"), NULL);
}

static void scanning(void) {
    int first = 0;
    int second = 0;
    unsigned hex = 0;
    double value = 0;
    char word[16] = "";
    int count = sscanf("  12 -7 ff 2.5e1 word", "%d %d %x %lf %15s", &first, &second, &hex, &value, word);
    check("sscanf", count == 5 && first == 12 && second == -7 && hex == 255 && value == 25.0 && !strcmp(word, "word"), NULL);
    WCHAR wide_word[16];
    int wide_value = 0;
    check("swscanf", swscanf(L"99 text", L"%d %15s", &wide_value, wide_word) == 2 && wide_value == 99 && !wcscmp(wide_word, L"text"), NULL);
    char *end;
    double tenth = strtod("0.1x", &end);
    unsigned long long bits;
    memcpy(&bits, &tenth, sizeof bits);
    check("strtod 0.1", bits == 0x3FB999999999999Aull && *end == 'x', NULL);
    check("strtol", strtol("  -0x1F", &end, 0) == -31 && !*end, NULL);
    check("strtoul", strtoul("4294967295", NULL, 10) == 4294967295ul, NULL);
    check("atoi", atoi("  123abc") == 123, NULL);
    check("strtoll", strtoll("-9223372036854775807", NULL, 10) == -9223372036854775807ll, NULL);
}

static void memory(void) {
    unsigned char *block = malloc(1000);
    int ok = block != NULL;
    for (int index = 0; ok && index < 1000; index++) {
        block[index] = (unsigned char)index;
    }
    block = ok ? realloc(block, 5000) : NULL;
    for (int index = 0; block && index < 1000; index++) {
        ok &= block[index] == (unsigned char)index;
    }
    check("malloc and realloc", ok && block && _msize(block) >= 5000, NULL);
    free(block);
    int *zeros = calloc(100, sizeof(int));
    ok = zeros != NULL;
    for (int index = 0; ok && index < 100; index++) {
        ok &= zeros[index] == 0;
    }
    check("calloc", ok, NULL);
    free(zeros);
    int numbers[] = {5, 3, 9, 1, 7, 2, 8, 6, 4, 0};
    qsort(numbers, 10, sizeof(int), compare_ints);
    ok = 1;
    for (int index = 0; index < 10; index++) {
        ok &= numbers[index] == index;
    }
    check("qsort", ok, NULL);
}

static void strings(void) {
    char text[64];
    strcpy(text, "Hello");
    strcat(text, ", World");
    check("strcpy and strcat", !strcmp(text, "Hello, World") && strlen(text) == 12, NULL);
    check("strstr and strchr", strstr(text, "World") == text + 7 && strchr(text, ',') == text + 5 && strrchr(text, 'o') == text + 8, NULL);
    check("_stricmp", !_stricmp("HeLLo", "hello") && _strnicmp("abcX", "ABCy", 3) == 0, NULL);
    char tokens[] = "one, two,,three";
    char *first = strtok(tokens, ", ");
    char *second = strtok(NULL, ", ");
    char *third = strtok(NULL, ", ");
    check("strtok", first && second && third && !strcmp(first, "one") && !strcmp(second, "two") && !strcmp(third, "three") && !strtok(NULL, ", "), NULL);
    char *copy = _strdup("Copy");
    check("_strdup and _strupr", copy && !strcmp(_strupr(copy), "COPY"), NULL);
    free(copy);
    check("ctype", isalpha('a') && isdigit('7') && !isalpha('7') && isspace('\t') && toupper('q') == 'Q', NULL);
    _itoa(-255, text, 16);
    check_text("_itoa base 16", text, "ffffff01");
    _itoa(-255, text, 10);
    check_text("_itoa base 10", text, "-255");
}

static void maths(void) {
    check_close("sin", sin(1.0), 0.8414709848078965);
    check_close("cos", cos(1.0), 0.54030230586813977);
    check_close("tan", tan(0.5), 0.54630248984379048);
    check_close("exp", exp(1.0), 2.7182818284590451);
    check_close("log", log(10.0), 2.3025850929940459);
    check_close("log10", log10(1000.0), 3.0);
    check_close("pow", pow(2.0, 0.5), 1.4142135623730951);
    check_close("sqrt", sqrt(2.0), 1.4142135623730951);
    check_close("atan2", atan2(1.0, 1.0) * 4, 3.1415926535897931);
    check_close("asin", asin(0.5), 0.52359877559829893);
    check_close("sinh", sinh(1.0), 1.1752011936438014);
    check_close("tanh", tanh(0.5), 0.46211715726000974);
    check_close("fmod", fmod(10.5, 3.0), 1.5);
    check_close("floor and ceil", floor(-1.5) + ceil(1.2), 0.0);
    check_close("sinf", (double)sinf(1.0f), (double)(float)0.8414709848078965);
    check_close("_hypot", _hypot(3.0, 4.0), 5.0);
}

static void files(void) {
    FILE *file = fopen("\\libc-test.txt", "w");
    check("fopen for writing", file != NULL, NULL);
    if (!file) {
        return;
    }
    fprintf(file, "line %d\n%s %.2f\n", 1, "pi", 3.14159);
    fputs("last", file);
    fclose(file);
    file = fopen("\\libc-test.txt", "r");
    check("fopen for reading", file != NULL, NULL);
    if (!file) {
        return;
    }
    char text[64] = "";
    fgets(text, sizeof text, file);
    check_text("fgets", text, "line 1\n");
    char word[16] = "";
    double value = 0;
    check("fscanf", fscanf(file, "%15s %lf", word, &value) == 2 && !strcmp(word, "pi") && value == 3.14, NULL);
    fseek(file, 0, SEEK_END);
    check("ftell in text mode", ftell(file) == 21, NULL);
    fseek(file, -4, SEEK_END);
    check("fread", fread(text, 1, 10, file) == 4 && !strncmp(text, "last", 4) && feof(file), NULL);
    fclose(file);
    check("remove", remove("\\libc-test.txt") == 0 && !fopen("\\libc-test.txt", "r"), NULL);
    printf("libc: printf to stdout %d\n", 42);
}

static void jumps(void) {
    volatile int counter = 0;
    int value = setjmp(jump);
    counter++;
    if (value == 0) {
        jump_back(7);
    }
    check("setjmp and longjmp", value == 7 && counter == 2, NULL);
}

static void posix(void) {
    char text[16] = "";
    struct stat status;
    int file = open("/posix-test.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    check("open for writing", file >= 3, NULL);
    check("write", write(file, "0123456789", 10) == 10, NULL);
    check("close", close(file) == 0 && close(file) == -1 && errno == EBADF, NULL);
    check("open O_EXCL", open("/posix-test.bin", O_WRONLY | O_CREAT | O_EXCL, 0644) == -1 && errno == EEXIST, NULL);
    file = open("/posix-test.bin", O_RDWR);
    check("lseek", lseek(file, 0, SEEK_END) == 10 && lseek(file, 2, SEEK_SET) == 2 && lseek(file, 1, SEEK_CUR) == 3, NULL);
    check("read", read(file, text, 4) == 4 && !memcmp(text, "3456", 4), NULL);
    check("ftruncate and fstat", ftruncate(file, 5) == 0 && fstat(file, &status) == 0 && status.st_size == 5 && S_ISREG(status.st_mode), NULL);
    check("read at the end", lseek(file, 0, SEEK_END) == 5 && read(file, text, 4) == 0, NULL);
    close(file);
    file = open("/posix-test.bin", O_WRONLY | O_APPEND);
    check("O_APPEND", write(file, "ab", 2) == 2 && lseek(file, 0, SEEK_CUR) == 7, NULL);
    close(file);
    check("stat", stat("/posix-test.bin", &status) == 0 && status.st_size == 7 && S_ISREG(status.st_mode) && status.st_mtime > 0, NULL);
    check("access", access("/posix-test.bin", R_OK | W_OK) == 0, NULL);
    check("unlink", unlink("/posix-test.bin") == 0 && access("/posix-test.bin", F_OK) == -1 && errno == ENOENT, NULL);
    check("open missing", open("/posix-test.bin", O_RDONLY) == -1 && errno == ENOENT, NULL);
    check("mkdir and stat", mkdir("/posix-test", 0755) == 0 && stat("/posix-test/", &status) == 0 && S_ISDIR(status.st_mode), NULL);
    check("rmdir", rmdir("/posix-test") == 0 && stat("/posix-test", &status) == -1, NULL);
    check("stat root", stat("/", &status) == 0 && S_ISDIR(status.st_mode), NULL);
    check("standard descriptors", write(STDOUT_FILENO, "libc: write to stdout\n", 22) == 22 && read(STDIN_FILENO, text, 1) == 0 && !isatty(1), NULL);
}

static void folders(void) {
    char original[MAX_PATH * 2];
    char text[MAX_PATH * 2];
    check("getcwd", getcwd(original, sizeof original) && original[0] == '\\', original);
    check("chdir to the root", chdir("/") == 0 && getcwd(text, sizeof text) && !strcmp(text, "\\"), text);
    check("mkdir and chdir", mkdir("posix-dir", 0755) == 0 && chdir("posix-dir/./") == 0 && getcwd(text, sizeof text) && !strcmp(text, "\\posix-dir"), text);
    FILE *file = fopen("relative.txt", "w");
    check("relative fopen", file != NULL, NULL);
    if (file) {
        fputs("relative", file);
        check("fileno", fileno(file) >= 3 && fileno(stdout) == 1, NULL);
        fflush(file);
        struct stat opened;
        check("fstat on fileno", fstat(fileno(file), &opened) == 0 && opened.st_size == 8 && S_ISREG(opened.st_mode), NULL);
        fclose(file);
    }
    struct stat status;
    check("relative file lands in the folder", stat("/posix-dir/relative.txt", &status) == 0 && status.st_size == 8, NULL);
    file = fopen("relative.txt", "r");
    char word[16] = "";
    check("rewind", file && fgetc(file) == 'r' && (rewind(file), fread(word, 1, 8, file)) == 8 && !memcmp(word, "relative", 8), NULL);
    if (file) {
        fclose(file);
    }
    DIR *directory = opendir(".");
    int found = 0;
    struct dirent *entry;
    while (directory && (entry = readdir(directory))) {
        found += !strcmp(entry->d_name, "relative.txt") && entry->d_type == DT_REG;
    }
    if (directory) {
        rewinddir(directory);
        found += readdir(directory) != NULL;
        closedir(directory);
    }
    check("opendir and readdir", found == 2, NULL);
    check("relative rename", rename("relative.txt", "../posix-dir/moved.txt") == 0 && access("moved.txt", F_OK) == 0 && access("relative.txt", F_OK) == -1, NULL);
    check("relative remove", remove("moved.txt") == 0, NULL);
    check("chdir up and rmdir", chdir("..") == 0 && rmdir("posix-dir") == 0 && opendir("posix-dir") == NULL && errno == ENOENT, NULL);
    check("chdir to a file fails", chdir("/posix-dir-missing") == -1, NULL);
    chdir(original);
}

static void at_exit(void) {
}

static volatile char constructed[4];

__attribute__((constructor(150))) static void construct_second(void) {
    constructed[1] = constructed[0] ? 'b' : '?';
}

__attribute__((constructor(120))) static void construct_first(void) {
    constructed[0] = 'a';
}

__attribute__((constructor)) static void construct_last(void) {
    constructed[2] = constructed[1] ? 'c' : '?';
}

#if __has_feature(c_thread_local)
static _Thread_local int thread_value = 5;

static DWORD WINAPI other_thread(LPVOID argument) {
    int seen = thread_value;
    thread_value = 9;
    return seen;
}
#endif

static volatile sig_atomic_t signalled;

static void on_signal(int number) {
    signalled = number;
}

static void library(void) {
    char text[64];
    struct stat status;
    time_t epoch = 0;
    check("time", time(NULL) > 0, NULL);
    strftime(text, sizeof text, "%Y-%m-%d %H:%M:%S %a", gmtime(&epoch));
    check_text("gmtime and strftime", text, "1970-01-01 00:00:00 Thu");
    time_t now = time(NULL);
    struct tm local = *localtime(&now);
    check("mktime", mktime(&local) == now, NULL);
    struct timeval first;
    struct timeval second;
    gettimeofday(&first, NULL);
    usleep(20000);
    gettimeofday(&second, NULL);
    long long elapsed = (second.tv_sec - first.tv_sec) * 1000000LL + (second.tv_usec - first.tv_usec);
    check("gettimeofday and usleep", elapsed >= 10000 && elapsed < 2000000, NULL);
    check("getenv and setenv", getenv("GRIMACE_TEST") == NULL && setenv("GRIMACE_TEST", "yes", 1) == 0 && !strcmp(getenv("GRIMACE_TEST"), "yes"), NULL);
    check("strcasecmp", strcasecmp("Hello", "hELLO") == 0 && strncasecmp("abcX", "ABCy", 3) == 0 && strcasecmp("a", "B") < 0, NULL);
    char *copy = strndup("abcdef", 3);
    check("strdup, strndup and strnlen", copy && !strcmp(copy, "abc") && strnlen("abcdef", 4) == 4 && strnlen("ab", 4) == 2, NULL);
    free(copy);
    copy = strdup("xyz");
    check("strdup", copy && !strcmp(copy, "xyz"), NULL);
    free(copy);
    check("strtoimax", strtoimax("-123456789012", NULL, 10) == -123456789012LL && strtoumax("ff", NULL, 16) == 255, NULL);
    char *stack = alloca(32);
    strcpy(stack, "alloca");
    check("alloca", !strcmp(stack, "alloca"), NULL);
    check("atexit", atexit(at_exit) == 0, NULL);
    check("C constructors in priority order", constructed[0] == 'a' && constructed[1] == 'b' && constructed[2] == 'c', NULL);
    assert(strlen("assert") == 6);
    static_assert(sizeof(int) == 4, "int is 32-bit");
    check("setlocale and localeconv", !strcmp(setlocale(LC_ALL, ""), "C") && setlocale(LC_ALL, "de_DE") == NULL && !strcmp(localeconv()->decimal_point, "."), NULL);
    check("signal and raise", signal(SIGINT, on_signal) == SIG_DFL && raise(SIGINT) == 0 && signalled == SIGINT && signal(SIGTERM, SIG_IGN) == SIG_DFL && raise(SIGTERM) == 0, NULL);
    struct sigaction action = {0};
    struct sigaction previous;
    sigset_t set;
    action.sa_handler = on_signal;
    check("sigaction", sigemptyset(&set) == 0 && sigaddset(&set, SIGPIPE) == 0 && sigismember(&set, SIGPIPE) == 1 && sigaction(SIGPIPE, &action, NULL) == 0 &&
                           sigaction(SIGPIPE, NULL, &previous) == 0 && previous.sa_handler == on_signal && raise(SIGPIPE) == 0 && signalled == SIGPIPE, NULL);
    check("fenv", fegetround() == FE_TONEAREST && fetestexcept(FE_ALL_EXCEPT) == 0, NULL);
    check("wctype", iswalpha(L'a') && towupper(L'a') == L'A', NULL);
    static const int sorted[] = {2, 3, 5, 7, 11, 13};
    int wanted = 7;
    int missing = 4;
    check("bsearch", bsearch(&wanted, sorted, 6, sizeof(int), compare_ints) == &sorted[3] && !bsearch(&missing, sorted, 6, sizeof(int), compare_ints), NULL);
    volatile double zero = 0.0;
    check("math classification", isnan(zero / zero) && isinf(1.0 / zero) && isfinite(1.0) && fpclassify(0.0) == FP_ZERO && signbit(-1.0) && !signbit(1.0), NULL);
    struct timespec monotonic;
    struct timespec realtime;
    check("clock_gettime", clock_gettime(CLOCK_MONOTONIC, &monotonic) == 0 && clock_gettime(CLOCK_REALTIME, &realtime) == 0 && realtime.tv_sec > 0, NULL);
    check("ids, PATH_MAX and MIN", getpid() != 0 && getuid() == 0 && getpwuid(0) == NULL && PATH_MAX == MAXPATHLEN && MIN(3, 4) == 3, NULL);
    int file = open("/posix-dup.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    int duplicate = dup(file);
    check("dup shares the position", duplicate >= 3 && duplicate != file && write(file, "abcdef", 6) == 6 && lseek(duplicate, 0, SEEK_CUR) == 6, NULL);
    close(file);
    check("dup outlives the original", lseek(duplicate, 0, SEEK_SET) == 0 && read(duplicate, text, 3) == 3 && !memcmp(text, "abc", 3), NULL);
    FILE *stream = fdopen(duplicate, "r+b");
    char letters[4] = "";
    check("fdopen", stream && fread(letters, 1, 3, stream) == 3 && !memcmp(letters, "def", 3) && ftello(stream) == 6 && fseeko(stream, 1, SEEK_SET) == 0, NULL);
    if (stream) {
        fclose(stream);
    }
    check("truncate", truncate("/posix-dup.bin", 2) == 0 && stat("/posix-dup.bin", &status) == 0 && status.st_size == 2, NULL);
    struct utimbuf times = {1000000000, 1000000000};
    check("utime", utime("/posix-dup.bin", &times) == 0 && stat("/posix-dup.bin", &status) == 0 && status.st_mtime == 1000000000, NULL);
    unlink("/posix-dup.bin");
    check("setvbuf", setvbuf(stdout, NULL, _IOFBF, BUFSIZ) == 0, NULL);
#if __has_feature(c_thread_local)
    thread_value = 6;
    HANDLE thread = CreateThread(NULL, 0, other_thread, NULL, 0, NULL);
    DWORD seen = 0;
    if (thread) {
        WaitForSingleObject(thread, INFINITE);
        GetExitCodeThread(thread, &seen);
        CloseHandle(thread);
    }
    check("_Thread_local", thread && seen == 5 && thread_value == 6, NULL);
#endif
    check("system and wait", system(NULL) == 0 && system("dir") == -1 && wait(NULL) == -1 && errno == ECHILD, NULL);
}

static void redirect(void) {
    char text[128] = "";
    unlink("/libc-stdio.txt");
    check("grimace_redirect_stdio", grimace_redirect_stdio(L"\\libc-stdio.txt") == 0, NULL);
    printf("out %d\n", 1);
    fputs("err 2\n", stderr);
    write(STDERR_FILENO, "fd 3\n", 5);
    printf("out %d\n", 4);
    int file = open("/libc-stdio.txt", O_RDONLY);
    ssize_t length = file >= 0 ? read(file, text, sizeof text - 1) : -1;
    close(file);
    text[length > 0 ? length : 0] = 0;
    check_text("redirected output", text, "out 1\r\nerr 2\r\nfd 3\r\nout 4\r\n");
}

#pragma clang diagnostic ignored "-Watomic-alignment"

static void atomics(void) {
    static volatile unsigned char bytes[4] = {1, 2, 3, 4};
    static volatile unsigned short halves[2] = {0x1111, 0x2222};
    static volatile unsigned int word = 10;
    static volatile unsigned long long wide = 0x100000000ull;
    unsigned char expected_byte = 3;
    unsigned int expected_word = 99;
    unsigned long long expected_wide = 0x100000000ull;
#if !defined(__mips__) || __CLANG_ATOMIC_INT_LOCK_FREE < 2
    check("atomic 1-byte", __atomic_exchange_n(&bytes[2], 30, __ATOMIC_SEQ_CST) == 3 && __atomic_fetch_add(&bytes[1], 5, __ATOMIC_SEQ_CST) == 2 &&
                               !__atomic_compare_exchange_n(&bytes[2], &expected_byte, 40, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) && expected_byte == 30 &&
                               __atomic_load_n(&bytes[0], __ATOMIC_SEQ_CST) == 1 && bytes[1] == 7 && bytes[3] == 4, NULL);
    check("atomic 2-byte", __atomic_fetch_or(&halves[1], 0x4000, __ATOMIC_SEQ_CST) == 0x2222 && halves[1] == 0x6222 && halves[0] == 0x1111, NULL);
    check("atomic 4-byte", __atomic_fetch_sub(&word, 3, __ATOMIC_SEQ_CST) == 10 && __atomic_exchange_n(&word, 99, __ATOMIC_SEQ_CST) == 7 &&
                               __atomic_compare_exchange_n(&word, &expected_word, 5, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) && word == 5, NULL);
#endif
    __sync_synchronize();
    static struct {
        char letters[3];
    } triple = {{'a', 'b', 'c'}}, replacement = {{'x', 'y', 'z'}}, previous;
    __atomic_exchange(&triple, &replacement, &previous, __ATOMIC_SEQ_CST);
    check("atomic 3-byte", !memcmp(previous.letters, "abc", 3) && !memcmp(triple.letters, "xyz", 3), NULL);
#if !defined(__mips__) || __CLANG_ATOMIC_INT_LOCK_FREE < 2
    static _Atomic int counter = 5;
    counter += 2;
    check("_Atomic int", counter == 7, NULL);
#endif
    check("atomic 8-byte", __atomic_fetch_add(&wide, 1, __ATOMIC_SEQ_CST) == 0x100000000ull &&
                               !__atomic_compare_exchange_n(&wide, &expected_wide, 0, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) && expected_wide == 0x100000001ull &&
                               __atomic_load_n(&wide, __ATOMIC_SEQ_CST) == 0x100000001ull, NULL);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPWSTR command_line, int show) {
    results = CreateFileW(command_line[0] ? command_line : L"\\libc.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    formatting();
    scanning();
    memory();
    strings();
    maths();
    files();
    jumps();
    atomics();
    posix();
    folders();
    library();
    redirect();
    WCHAR text[64];
    swprintf(text, L"libc: %d passed, %d failed\r\n", passed, failed);
    line(text);
    wcscat(summary, text + 6);
    if (results != INVALID_HANDLE_VALUE) {
        CloseHandle(results);
    }
    MessageBoxW(NULL, summary, L"libc", MB_OK);
    return failed;
}
