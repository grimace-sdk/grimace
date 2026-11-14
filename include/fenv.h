#ifndef GRIMACE_FENV_H
#define GRIMACE_FENV_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int fenv_t;
typedef unsigned int fexcept_t;

#define FE_ALL_EXCEPT 0
#define FE_TONEAREST 0
#define FE_DFL_ENV ((const fenv_t *)0)

int feclearexcept(int exceptions);
int fegetexceptflag(fexcept_t *flags, int exceptions);
int feraiseexcept(int exceptions);
int fesetexceptflag(const fexcept_t *flags, int exceptions);
int fetestexcept(int exceptions);
int fegetround(void);
int fesetround(int rounding);
int fegetenv(fenv_t *environment);
int feholdexcept(fenv_t *environment);
int fesetenv(const fenv_t *environment);
int feupdateenv(const fenv_t *environment);

#ifdef __cplusplus
}
#endif

#endif
