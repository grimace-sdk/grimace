#include "internal.h"
#include <errno.h>
#include <sys/wait.h>

pid_t wait(int *status) {
    return __grimace_posix_fail(ECHILD);
}

pid_t waitpid(pid_t process, int *status, int options) {
    return __grimace_posix_fail(ECHILD);
}
