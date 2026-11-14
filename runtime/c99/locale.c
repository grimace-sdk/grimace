#include <stddef.h>

struct lconv {
    char *decimal_point;
    char *thousands_sep;
    char *grouping;
    char *int_curr_symbol;
    char *currency_symbol;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *positive_sign;
    char *negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char int_p_cs_precedes;
    char int_n_cs_precedes;
    char int_p_sep_by_space;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
};

static char c_locale[] = "C";
static char point[] = ".";
static char empty[] = "";

static struct lconv conventions = {point, empty, empty, empty, empty, empty, empty, empty, empty, empty, 127, 127,
                                   127,   127,   127,   127,   127,   127,   127,   127,   127,   127,   127, 127};

char *setlocale(int category, const char *locale) {
    if (category < 0 || category > 5) {
        return NULL;
    }
    if (!locale || !locale[0] || (locale[0] == 'C' && !locale[1])) {
        return c_locale;
    }
    const char *posix = "POSIX";
    for (int index = 0;; index++) {
        if (locale[index] != posix[index]) {
            return NULL;
        }
        if (!locale[index]) {
            return c_locale;
        }
    }
}

struct lconv *localeconv(void) {
    return &conventions;
}
