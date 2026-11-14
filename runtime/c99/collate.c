#include <stddef.h>

typedef unsigned short wchar_type;

int strcoll(const char *first, const char *second) {
    const unsigned char *left = (const unsigned char *)first;
    const unsigned char *right = (const unsigned char *)second;
    while (*left && *left == *right) {
        left++;
        right++;
    }
    return *left - *right;
}

size_t strxfrm(char *destination, const char *source, size_t count) {
    size_t length = 0;
    while (source[length]) {
        length++;
    }
    for (size_t index = 0; index < count && index <= length; index++) {
        destination[index] = source[index];
    }
    return length;
}

int wcscoll(const wchar_type *first, const wchar_type *second) {
    while (*first && *first == *second) {
        first++;
        second++;
    }
    return *first - *second;
}

size_t wcsxfrm(wchar_type *destination, const wchar_type *source, size_t count) {
    size_t length = 0;
    while (source[length]) {
        length++;
    }
    for (size_t index = 0; index < count && index <= length; index++) {
        destination[index] = source[index];
    }
    return length;
}
