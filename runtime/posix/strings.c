#include <strings.h>

static int lower(unsigned char character) {
    return character >= 'A' && character <= 'Z' ? character + ('a' - 'A') : character;
}

int strncasecmp(const char *first, const char *second, size_t count) {
    for (; count; count--, first++, second++) {
        int difference = lower(*first) - lower(*second);
        if (difference || !*first) {
            return difference;
        }
    }
    return 0;
}

int strcasecmp(const char *first, const char *second) {
    return strncasecmp(first, second, (size_t)-1);
}
