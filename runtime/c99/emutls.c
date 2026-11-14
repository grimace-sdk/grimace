#include <stddef.h>
#include <stdint.h>

#define TLS_OUT_OF_INDEXES 0xFFFFFFFFu
#define TLS_FUNCALLOC 0
#define TLS_FUNCFREE 1

unsigned long TlsCall(unsigned long function, unsigned long value);
void *TlsGetValue(unsigned long slot);
int TlsSetValue(unsigned long slot, void *value);
void *malloc(size_t size);
void *realloc(void *memory, size_t size);
void *memcpy(void *destination, const void *source, size_t count);
void *memset(void *destination, int value, size_t count);
__attribute__((noreturn)) void abort(void);

typedef struct {
    size_t size;
    size_t align;
    uintptr_t index;
    void *value;
} emutls_control;

typedef struct {
    size_t count;
    void *objects[1];
} thread_objects;

uint32_t fetch_add_word(volatile void *address, uint32_t operand, int order) __asm__("__atomic_fetch_add_4");
_Bool compare_exchange_word(volatile void *address, void *expected, uint32_t desired, int success, int failure)
    __asm__("__atomic_compare_exchange_4");

static volatile uint32_t next_index;
static volatile uint32_t slot = TLS_OUT_OF_INDEXES;

static unsigned long thread_slot(void) {
    if (slot == TLS_OUT_OF_INDEXES) {
        uint32_t allocated = TlsCall(TLS_FUNCALLOC, 0);
        uint32_t expected = TLS_OUT_OF_INDEXES;
        if (!compare_exchange_word(&slot, &expected, allocated, 5, 5)) {
            TlsCall(TLS_FUNCFREE, allocated);
        }
    }
    return slot;
}

static uintptr_t object_index(emutls_control *control) {
    uint32_t index = *(volatile uint32_t *)&control->index;
    if (!index) {
        uint32_t allocated = fetch_add_word(&next_index, 1, 5) + 1;
        uint32_t expected = 0;
        index = compare_exchange_word(&control->index, &expected, allocated, 5, 5) ? allocated : expected;
    }
    return index;
}

static void *new_object(emutls_control *control) {
    size_t align = control->align < sizeof(void *) ? sizeof(void *) : control->align;
    char *block = malloc(control->size + align + sizeof(void *));
    if (!block) {
        abort();
    }
    uintptr_t start = ((uintptr_t)block + sizeof(void *) + align - 1) & ~(uintptr_t)(align - 1);
    void *object = (void *)start;
    if (control->value) {
        memcpy(object, control->value, control->size);
    } else {
        memset(object, 0, control->size);
    }
    return object;
}

void *__emutls_get_address(emutls_control *control) {
    unsigned long tls = thread_slot();
    uintptr_t index = object_index(control);
    thread_objects *objects = TlsGetValue(tls);
    if (!objects || objects->count < index) {
        size_t old_count = objects ? objects->count : 0;
        size_t new_count = index + 16;
        thread_objects *grown = realloc(objects, sizeof(thread_objects) + new_count * sizeof(void *));
        if (!grown) {
            abort();
        }
        size_t first_new = objects ? old_count + 1 : 0;
        memset(grown->objects + first_new, 0, (new_count + 1 - first_new) * sizeof(void *));
        grown->count = new_count;
        objects = grown;
        TlsSetValue(tls, objects);
    }
    void *object = objects->objects[index];
    if (!object) {
        object = new_object(control);
        objects->objects[index] = object;
    }
    return object;
}
