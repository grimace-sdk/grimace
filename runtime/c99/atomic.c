#include <stddef.h>
#include <stdint.h>

void *memcpy(void *destination, const void *source, size_t count);
int memcmp(const void *left, const void *right, size_t count);

void Sleep(unsigned long milliseconds);

#define BARRIER() __asm__ volatile("" ::: "memory")

#if defined(__i386__)
static uint32_t swap_word(volatile uint32_t *word, uint32_t value) {
    return __atomic_exchange_n(word, value, __ATOMIC_SEQ_CST);
}

static uint32_t test_exchange_word(volatile uint32_t *word, uint32_t expected, uint32_t desired) {
    __atomic_compare_exchange_n(word, &expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return expected;
}
#else
long InterlockedExchange(volatile long *target, long value);
long InterlockedTestExchange(volatile long *target, long old_value, long new_value);

static uint32_t swap_word(volatile uint32_t *word, uint32_t value) {
    return InterlockedExchange((volatile long *)word, value);
}

static uint32_t test_exchange_word(volatile uint32_t *word, uint32_t expected, uint32_t desired) {
    return InterlockedTestExchange((volatile long *)word, expected, desired);
}
#endif

void synchronize(void) __asm__("__sync_synchronize");

void synchronize(void) {
    BARRIER();
}

static int compare_exchange_word(volatile uint32_t *word, uint32_t *expected, uint32_t desired) {
    uint32_t previous = test_exchange_word(word, *expected, desired);
    if (previous == *expected) {
        return 1;
    }
    *expected = previous;
    return 0;
}

static volatile uint32_t *containing_word(volatile void *address, unsigned *shift) {
    uintptr_t byte_address = (uintptr_t)address;
    *shift = (byte_address & 3) * 8;
    return (volatile uint32_t *)(byte_address & ~(uintptr_t)3);
}

static uint32_t update_part(volatile void *address, uint32_t mask, uint32_t (*update)(uint32_t, uint32_t), uint32_t operand) {
    unsigned shift;
    volatile uint32_t *word = containing_word(address, &shift);
    uint32_t current = *word;
    for (;;) {
        uint32_t part = (current >> shift) & mask;
        uint32_t replacement = (current & ~(mask << shift)) | ((update(part, operand) & mask) << shift);
        if (compare_exchange_word(word, &current, replacement)) {
            return part;
        }
    }
}

static int compare_exchange_part(volatile void *address, uint32_t mask, uint32_t *expected, uint32_t desired) {
    unsigned shift;
    volatile uint32_t *word = containing_word(address, &shift);
    uint32_t current = *word;
    for (;;) {
        uint32_t part = (current >> shift) & mask;
        if (part != *expected) {
            *expected = part;
            return 0;
        }
        uint32_t replacement = (current & ~(mask << shift)) | ((desired & mask) << shift);
        if (compare_exchange_word(word, &current, replacement)) {
            return 1;
        }
    }
}

static uint32_t update_word(volatile uint32_t *word, uint32_t (*update)(uint32_t, uint32_t), uint32_t operand) {
    uint32_t current = *word;
    while (!compare_exchange_word(word, &current, update(current, operand))) {
    }
    return current;
}

static uint32_t replace32(uint32_t value, uint32_t operand) {
    return operand;
}

static uint32_t add32(uint32_t value, uint32_t operand) {
    return value + operand;
}

static uint32_t subtract32(uint32_t value, uint32_t operand) {
    return value - operand;
}

static uint32_t and32(uint32_t value, uint32_t operand) {
    return value & operand;
}

static uint32_t or32(uint32_t value, uint32_t operand) {
    return value | operand;
}

static uint32_t xor32(uint32_t value, uint32_t operand) {
    return value ^ operand;
}

static uint32_t nand32(uint32_t value, uint32_t operand) {
    return ~(value & operand);
}

#define PART_FUNCTIONS(size, type, mask)                                                                           \
    type atomic_load_##size(const volatile void *address, int order) __asm__("__atomic_load_" #size);             \
    type atomic_load_##size(const volatile void *address, int order) {                                           \
        BARRIER();                                                                                                \
        type value = *(const volatile type *)address;                                                             \
        BARRIER();                                                                                                \
        return value;                                                                                             \
    }                                                                                                             \
    void atomic_store_##size(volatile void *address, type value, int order) __asm__("__atomic_store_" #size);     \
    void atomic_store_##size(volatile void *address, type value, int order) {                                    \
        BARRIER();                                                                                                \
        *(volatile type *)address = value;                                                                        \
        BARRIER();                                                                                                \
    }                                                                                                             \
    type atomic_exchange_##size(volatile void *address, type value, int order) __asm__("__atomic_exchange_" #size); \
    type atomic_exchange_##size(volatile void *address, type value, int order) {                                 \
        return update_part(address, mask, replace32, value);                                                      \
    }                                                                                                             \
    _Bool atomic_compare_exchange_##size(volatile void *address, void *expected, type desired, int success,      \
                                         int failure) __asm__("__atomic_compare_exchange_" #size);                \
    _Bool atomic_compare_exchange_##size(volatile void *address, void *expected, type desired, int success,      \
                                         int failure) {                                                           \
        uint32_t expected_part = *(type *)expected;                                                               \
        int exchanged = compare_exchange_part(address, mask, &expected_part, desired);                            \
        *(type *)expected = expected_part;                                                                        \
        return exchanged;                                                                                         \
    }                                                                                                             \
    PART_FETCH(size, type, mask, add, add32)                                                                      \
    PART_FETCH(size, type, mask, sub, subtract32)                                                                 \
    PART_FETCH(size, type, mask, and, and32)                                                                      \
    PART_FETCH(size, type, mask, or, or32)                                                                        \
    PART_FETCH(size, type, mask, xor, xor32)                                                                      \
    PART_FETCH(size, type, mask, nand, nand32)

#define PART_FETCH(size, type, mask, name, update)                                                                 \
    type atomic_fetch_##name##_##size(volatile void *address, type operand, int order)                           \
        __asm__("__atomic_fetch_" #name "_" #size);                                                               \
    type atomic_fetch_##name##_##size(volatile void *address, type operand, int order) {                         \
        return update_part(address, mask, update, operand);                                                      \
    }

PART_FUNCTIONS(1, uint8_t, 0xFF)
PART_FUNCTIONS(2, uint16_t, 0xFFFF)

#define WORD_FETCH(name, update)                                                                                   \
    uint32_t atomic_fetch_##name##_4(volatile void *address, uint32_t operand, int order)                        \
        __asm__("__atomic_fetch_" #name "_4");                                                                    \
    uint32_t atomic_fetch_##name##_4(volatile void *address, uint32_t operand, int order) {                      \
        return update_word(address, update, operand);                                                             \
    }

uint32_t atomic_load_4(const volatile void *address, int order) __asm__("__atomic_load_4");

uint32_t atomic_load_4(const volatile void *address, int order) {
    BARRIER();
    uint32_t value = *(const volatile uint32_t *)address;
    BARRIER();
    return value;
}

void atomic_store_4(volatile void *address, uint32_t value, int order) __asm__("__atomic_store_4");

void atomic_store_4(volatile void *address, uint32_t value, int order) {
    BARRIER();
    *(volatile uint32_t *)address = value;
    BARRIER();
}

uint32_t atomic_exchange_4(volatile void *address, uint32_t value, int order) __asm__("__atomic_exchange_4");

uint32_t atomic_exchange_4(volatile void *address, uint32_t value, int order) {
    return swap_word(address, value);
}

_Bool atomic_compare_exchange_4(volatile void *address, void *expected, uint32_t desired, int success, int failure)
    __asm__("__atomic_compare_exchange_4");

_Bool atomic_compare_exchange_4(volatile void *address, void *expected, uint32_t desired, int success, int failure) {
    return compare_exchange_word(address, expected, desired);
}

WORD_FETCH(add, add32)
WORD_FETCH(sub, subtract32)
WORD_FETCH(and, and32)
WORD_FETCH(or, or32)
WORD_FETCH(xor, xor32)
WORD_FETCH(nand, nand32)

static volatile uint32_t long_lock;

static void lock(void) {
    while (swap_word(&long_lock, 1)) {
        Sleep(0);
    }
    BARRIER();
}

static void unlock(void) {
    BARRIER();
    swap_word(&long_lock, 0);
}

uint64_t atomic_load_8(const volatile void *address, int order) __asm__("__atomic_load_8");

uint64_t atomic_load_8(const volatile void *address, int order) {
    lock();
    uint64_t value = *(const volatile uint64_t *)address;
    unlock();
    return value;
}

void atomic_store_8(volatile void *address, uint64_t value, int order) __asm__("__atomic_store_8");

void atomic_store_8(volatile void *address, uint64_t value, int order) {
    lock();
    *(volatile uint64_t *)address = value;
    unlock();
}

uint64_t atomic_exchange_8(volatile void *address, uint64_t value, int order) __asm__("__atomic_exchange_8");

uint64_t atomic_exchange_8(volatile void *address, uint64_t value, int order) {
    lock();
    uint64_t previous = *(volatile uint64_t *)address;
    *(volatile uint64_t *)address = value;
    unlock();
    return previous;
}

_Bool atomic_compare_exchange_8(volatile void *address, void *expected, uint64_t desired, int success, int failure)
    __asm__("__atomic_compare_exchange_8");

_Bool atomic_compare_exchange_8(volatile void *address, void *expected, uint64_t desired, int success, int failure) {
    lock();
    uint64_t current = *(volatile uint64_t *)address;
    int exchanged = current == *(uint64_t *)expected;
    if (exchanged) {
        *(volatile uint64_t *)address = desired;
    } else {
        *(uint64_t *)expected = current;
    }
    unlock();
    return exchanged;
}

#define LONG_FETCH(name, expression)                                                                               \
    uint64_t atomic_fetch_##name##_8(volatile void *address, uint64_t operand, int order)                        \
        __asm__("__atomic_fetch_" #name "_8");                                                                    \
    uint64_t atomic_fetch_##name##_8(volatile void *address, uint64_t operand, int order) {                      \
        lock();                                                                                                   \
        uint64_t value = *(volatile uint64_t *)address;                                                           \
        *(volatile uint64_t *)address = expression;                                                               \
        unlock();                                                                                                 \
        return value;                                                                                             \
    }

LONG_FETCH(add, value + operand)
LONG_FETCH(sub, value - operand)
LONG_FETCH(and, value & operand)
LONG_FETCH(or, value | operand)
LONG_FETCH(xor, value ^ operand)
LONG_FETCH(nand, ~(value & operand))

static int sized(size_t size, const volatile void *address) {
    return (size == 1 || size == 2 || size == 4 || size == 8) && !((uintptr_t)address & (size - 1));
}

void atomic_load(size_t size, const volatile void *address, void *result, int order) __asm__("__atomic_load");

void atomic_load(size_t size, const volatile void *address, void *result, int order) {
    if (!sized(size, address)) {
        lock();
        memcpy(result, (const void *)address, size);
        unlock();
    } else if (size == 1) {
        *(uint8_t *)result = atomic_load_1(address, order);
    } else if (size == 2) {
        *(uint16_t *)result = atomic_load_2(address, order);
    } else if (size == 4) {
        *(uint32_t *)result = atomic_load_4(address, order);
    } else {
        *(uint64_t *)result = atomic_load_8(address, order);
    }
}

void atomic_store(size_t size, volatile void *address, void *value, int order) __asm__("__atomic_store");

void atomic_store(size_t size, volatile void *address, void *value, int order) {
    if (!sized(size, address)) {
        lock();
        memcpy((void *)address, value, size);
        unlock();
    } else if (size == 1) {
        atomic_store_1(address, *(uint8_t *)value, order);
    } else if (size == 2) {
        atomic_store_2(address, *(uint16_t *)value, order);
    } else if (size == 4) {
        atomic_store_4(address, *(uint32_t *)value, order);
    } else {
        atomic_store_8(address, *(uint64_t *)value, order);
    }
}

void atomic_exchange(size_t size, volatile void *address, void *value, void *previous, int order) __asm__("__atomic_exchange");

void atomic_exchange(size_t size, volatile void *address, void *value, void *previous, int order) {
    if (!sized(size, address)) {
        lock();
        memcpy(previous, (const void *)address, size);
        memcpy((void *)address, value, size);
        unlock();
    } else if (size == 1) {
        *(uint8_t *)previous = atomic_exchange_1(address, *(uint8_t *)value, order);
    } else if (size == 2) {
        *(uint16_t *)previous = atomic_exchange_2(address, *(uint16_t *)value, order);
    } else if (size == 4) {
        *(uint32_t *)previous = atomic_exchange_4(address, *(uint32_t *)value, order);
    } else {
        *(uint64_t *)previous = atomic_exchange_8(address, *(uint64_t *)value, order);
    }
}

_Bool atomic_compare_exchange(size_t size, volatile void *address, void *expected, void *desired, int success, int failure)
    __asm__("__atomic_compare_exchange");

_Bool atomic_compare_exchange(size_t size, volatile void *address, void *expected, void *desired, int success, int failure) {
    if (!sized(size, address)) {
        lock();
        int exchanged = !memcmp((const void *)address, expected, size);
        if (exchanged) {
            memcpy((void *)address, desired, size);
        } else {
            memcpy(expected, (const void *)address, size);
        }
        unlock();
        return exchanged;
    }
    if (size == 1) {
        return atomic_compare_exchange_1(address, expected, *(uint8_t *)desired, success, failure);
    }
    if (size == 2) {
        return atomic_compare_exchange_2(address, expected, *(uint16_t *)desired, success, failure);
    }
    if (size == 4) {
        return atomic_compare_exchange_4(address, expected, *(uint32_t *)desired, success, failure);
    }
    return atomic_compare_exchange_8(address, expected, *(uint64_t *)desired, success, failure);
}
