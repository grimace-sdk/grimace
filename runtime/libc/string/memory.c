#include <stddef.h>
#include <stdint.h>

typedef uint32_t __attribute__((may_alias)) word;

static int co_aligned(const void *first, const void *second) {
    return (((uintptr_t)first ^ (uintptr_t)second) & 3) == 0;
}

static void copy_forward(unsigned char *output, const unsigned char *input, size_t count) {
    if (co_aligned(output, input)) {
        while (count && ((uintptr_t)output & 3)) {
            *output++ = *input++;
            count--;
        }
        word *words_out = (word *)output;
        const word *words_in = (const word *)input;
        while (count >= 16) {
            word first = words_in[0];
            word second = words_in[1];
            word third = words_in[2];
            word fourth = words_in[3];
            words_out[0] = first;
            words_out[1] = second;
            words_out[2] = third;
            words_out[3] = fourth;
            words_out += 4;
            words_in += 4;
            count -= 16;
        }
        while (count >= 4) {
            *words_out++ = *words_in++;
            count -= 4;
        }
        output = (unsigned char *)words_out;
        input = (const unsigned char *)words_in;
    }
    while (count--) {
        *output++ = *input++;
    }
}

static void copy_backward(unsigned char *output, const unsigned char *input, size_t count) {
    output += count;
    input += count;
    if (co_aligned(output, input)) {
        while (count && ((uintptr_t)output & 3)) {
            *--output = *--input;
            count--;
        }
        word *words_out = (word *)output;
        const word *words_in = (const word *)input;
        while (count >= 4) {
            *--words_out = *--words_in;
            count -= 4;
        }
        output = (unsigned char *)words_out;
        input = (const unsigned char *)words_in;
    }
    while (count--) {
        *--output = *--input;
    }
}

void *memcpy(void *destination, const void *source, size_t count) {
    copy_forward(destination, source, count);
    return destination;
}

void *memmove(void *destination, const void *source, size_t count) {
    unsigned char *output = destination;
    const unsigned char *input = source;
    if (output <= input || output >= input + count) {
        copy_forward(output, input, count);
    } else {
        copy_backward(output, input, count);
    }
    return destination;
}

void *memset(void *destination, int value, size_t count) {
    unsigned char *output = destination;
    unsigned char byte = (unsigned char)value;
    while (count && ((uintptr_t)output & 3)) {
        *output++ = byte;
        count--;
    }
    word pattern = byte * 0x01010101u;
    word *words = (word *)output;
    while (count >= 16) {
        words[0] = pattern;
        words[1] = pattern;
        words[2] = pattern;
        words[3] = pattern;
        words += 4;
        count -= 16;
    }
    while (count >= 4) {
        *words++ = pattern;
        count -= 4;
    }
    output = (unsigned char *)words;
    while (count--) {
        *output++ = byte;
    }
    return destination;
}

int memcmp(const void *left, const void *right, size_t count) {
    const unsigned char *a = left;
    const unsigned char *b = right;
    if (co_aligned(a, b)) {
        while (count && ((uintptr_t)a & 3)) {
            if (*a != *b) {
                return *a - *b;
            }
            a++;
            b++;
            count--;
        }
        while (count >= 4 && *(const word *)a == *(const word *)b) {
            a += 4;
            b += 4;
            count -= 4;
        }
    }
    for (; count; count--, a++, b++) {
        if (*a != *b) {
            return *a - *b;
        }
    }
    return 0;
}
