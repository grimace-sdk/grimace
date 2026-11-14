#include <stddef.h>

void *bsearch(const void *key, const void *base, size_t count, size_t size, int (*compare)(const void *, const void *)) {
    const char *first = base;
    while (count) {
        const char *middle = first + (count / 2) * size;
        int order = compare(key, middle);
        if (order == 0) {
            return (void *)middle;
        }
        if (order > 0) {
            first = middle + size;
            count -= count / 2 + 1;
        } else {
            count /= 2;
        }
    }
    return NULL;
}
