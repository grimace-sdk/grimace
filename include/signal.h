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

#ifdef __cplusplus
}
#endif

#endif
