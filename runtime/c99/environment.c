#include <stddef.h>

void *malloc(size_t size);
void free(void *memory);
size_t strlen(const char *string);
int strncmp(const char *first, const char *second, size_t count);
void *memcpy(void *destination, const void *source, size_t count);

struct variable {
    struct variable *next;
    size_t name_length;
    char text[1];
};

static struct variable *variables;

static size_t name_length(const char *name) {
    size_t length = 0;
    while (name[length] && name[length] != '=') {
        length++;
    }
    return length;
}

static struct variable **find(const char *name, size_t length) {
    for (struct variable **link = &variables; *link; link = &(*link)->next) {
        if ((*link)->name_length == length && !strncmp((*link)->text, name, length)) {
            return link;
        }
    }
    return NULL;
}

char *getenv(const char *name) {
    struct variable **link = find(name, strlen(name));
    return link ? (*link)->text + (*link)->name_length + 1 : NULL;
}

static int store(const char *name, size_t length, const char *value) {
    size_t value_length = strlen(value);
    struct variable *variable = malloc(sizeof *variable + length + value_length + 1);
    if (!variable) {
        return -1;
    }
    memcpy(variable->text, name, length);
    variable->text[length] = '=';
    memcpy(variable->text + length + 1, value, value_length + 1);
    variable->name_length = length;
    struct variable **existing = find(name, length);
    if (existing) {
        struct variable *previous = *existing;
        variable->next = previous->next;
        *existing = variable;
        free(previous);
    } else {
        variable->next = variables;
        variables = variable;
    }
    return 0;
}

int setenv(const char *name, const char *value, int overwrite) {
    size_t length = strlen(name);
    if (length == 0 || name_length(name) != length) {
        return -1;
    }
    if (!overwrite && find(name, length)) {
        return 0;
    }
    return store(name, length, value);
}

int unsetenv(const char *name) {
    size_t length = strlen(name);
    if (length == 0 || name_length(name) != length) {
        return -1;
    }
    struct variable **link = find(name, length);
    if (link) {
        struct variable *removed = *link;
        *link = removed->next;
        free(removed);
    }
    return 0;
}

int putenv(char *assignment) {
    size_t length = name_length(assignment);
    if (length == 0) {
        return -1;
    }
    if (!assignment[length]) {
        return unsetenv(assignment);
    }
    return store(assignment, length, assignment + length + 1);
}

int system(const char *command) {
    return command ? -1 : 0;
}
