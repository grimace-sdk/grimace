typedef unsigned int fenv_t;
typedef unsigned int fexcept_t;

int feclearexcept(int exceptions) {
    return exceptions ? -1 : 0;
}

int fegetexceptflag(fexcept_t *flags, int exceptions) {
    *flags = 0;
    return 0;
}

int feraiseexcept(int exceptions) {
    return exceptions ? -1 : 0;
}

int fesetexceptflag(const fexcept_t *flags, int exceptions) {
    return exceptions ? -1 : 0;
}

int fetestexcept(int exceptions) {
    return 0;
}

int fegetround(void) {
    return 0;
}

int fesetround(int rounding) {
    return rounding ? -1 : 0;
}

int fegetenv(fenv_t *environment) {
    *environment = 0;
    return 0;
}

int feholdexcept(fenv_t *environment) {
    *environment = 0;
    return 0;
}

int fesetenv(const fenv_t *environment) {
    return 0;
}

int feupdateenv(const fenv_t *environment) {
    return 0;
}
