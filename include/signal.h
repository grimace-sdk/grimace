#ifndef GRIMACE_SIGNAL_H
#define GRIMACE_SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int sig_atomic_t;
typedef void (*__grimace_signal_handler)(int);

#define SIGINT 2
#define SIGILL 4
#define SIGFPE 8
#define SIGSEGV 11
#define SIGTERM 15
#define SIGABRT 22

#define SIG_DFL ((__grimace_signal_handler)0)
#define SIG_IGN ((__grimace_signal_handler)1)
#define SIG_ERR ((__grimace_signal_handler)-1)

__grimace_signal_handler signal(int number, __grimace_signal_handler handler);
int raise(int number);

#ifdef GRIMACE_POSIX
#define SIGHUP 1
#define SIGQUIT 3
#define SIGTRAP 5
#define SIGBUS 7
#define SIGKILL 9
#define SIGUSR1 10
#define SIGUSR2 12
#define SIGPIPE 13
#define SIGALRM 14
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGWINCH 28

#define SA_NOCLDSTOP 1
#define SA_SIGINFO 4
#define SA_ONSTACK 0x08000000
#define SA_RESTART 0x10000000
#define SA_NODEFER 0x40000000
#define SA_RESETHAND 0x80000000

#define SIG_BLOCK 0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2

typedef unsigned long sigset_t;

typedef struct {
    int si_signo;
    int si_errno;
    int si_code;
} siginfo_t;

struct sigaction {
    union {
        __grimace_signal_handler handler;
        void (*action)(int, siginfo_t *, void *);
    } __grimace_handler;
    sigset_t sa_mask;
    int sa_flags;
};

#define sa_handler __grimace_handler.handler
#define sa_sigaction __grimace_handler.action

int sigaction(int number, const struct sigaction *action, struct sigaction *previous);
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int number);
int sigdelset(sigset_t *set, int number);
int sigismember(const sigset_t *set, int number);
int sigprocmask(int how, const sigset_t *set, sigset_t *previous);
int kill(int process, int number);
#endif

#ifdef __cplusplus
}
#endif

#endif
