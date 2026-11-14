#include "internal.h"
#include <errno.h>
#include <signal.h>

#define SIGNALS 32

int sigaction(int number, const struct sigaction *action, struct sigaction *previous) {
    if (number <= 0 || number >= SIGNALS || number == SIGKILL || number == SIGSTOP) {
        return __grimace_posix_fail(EINVAL);
    }
    __grimace_signal_handler old = action ? signal(number, action->sa_handler) : signal(number, SIG_DFL);
    if (!action) {
        signal(number, old);
    }
    if (previous) {
        previous->sa_handler = old;
        previous->sa_mask = 0;
        previous->sa_flags = 0;
    }
    return 0;
}

int sigemptyset(sigset_t *set) {
    *set = 0;
    return 0;
}

int sigfillset(sigset_t *set) {
    *set = ~0UL;
    return 0;
}

int sigaddset(sigset_t *set, int number) {
    if (number <= 0 || number >= SIGNALS) {
        return __grimace_posix_fail(EINVAL);
    }
    *set |= 1UL << number;
    return 0;
}

int sigdelset(sigset_t *set, int number) {
    if (number <= 0 || number >= SIGNALS) {
        return __grimace_posix_fail(EINVAL);
    }
    *set &= ~(1UL << number);
    return 0;
}

int sigismember(const sigset_t *set, int number) {
    if (number <= 0 || number >= SIGNALS) {
        return __grimace_posix_fail(EINVAL);
    }
    return (*set >> number) & 1;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *previous) {
    if (previous) {
        *previous = 0;
    }
    return 0;
}

int kill(int process, int number) {
    if (process == (int)GetCurrentProcessId() || process == 0) {
        return number ? raise(number) : 0;
    }
    return __grimace_posix_fail(EPERM);
}
