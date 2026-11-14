#include <stddef.h>
#include <string.h>

struct type_information {
    const void *vtable;
    const char *name;
};

struct single_inheritance_information {
    type_information base;
    const type_information *parent;
};

struct base_class_information {
    const type_information *type;
    long offset_flags;
};

struct multiple_inheritance_information {
    type_information base;
    unsigned int flags;
    unsigned int base_count;
    base_class_information bases[1];
};

enum {
    VIRTUAL_BASE = 1,
    PUBLIC_BASE = 2,
    MAXIMUM_FOUND = 8,
};

extern "C" {
const void *_ZTVN10__cxxabiv117__class_type_infoE[8] = {};
const void *_ZTVN10__cxxabiv120__si_class_type_infoE[8] = {};
const void *_ZTVN10__cxxabiv121__vmi_class_type_infoE[8] = {};
}

static bool is_kind(const type_information *type, const void **vtable) {
    return type->vtable == vtable + 2;
}

static bool same_type(const type_information *first, const type_information *second) {
    return first == second || first->name == second->name || strcmp(first->name, second->name) == 0;
}

static ptrdiff_t base_offset(const base_class_information &base, const char *object) {
    ptrdiff_t offset = base.offset_flags >> 8;
    if (base.offset_flags & VIRTUAL_BASE) {
        const char *vtable = *reinterpret_cast<const char *const *>(object);
        offset = *reinterpret_cast<const ptrdiff_t *>(vtable + offset);
    }
    return offset;
}

struct found_objects {
    const char *objects[MAXIMUM_FOUND];
    int count;
};

struct cast_search {
    const type_information *destination_type;
    const type_information *source_type;
    const char *source;
    found_objects containing_source;
    found_objects public_destinations;
};

static void remember(found_objects &found, const char *object) {
    for (int index = 0; index < found.count && index < MAXIMUM_FOUND; index++) {
        if (found.objects[index] == object) {
            return;
        }
    }
    if (found.count < MAXIMUM_FOUND) {
        found.objects[found.count] = object;
    }
    found.count++;
}

static bool contains_source(const type_information *type, const char *object, const cast_search &search) {
    if (object == search.source && same_type(type, search.source_type)) {
        return true;
    }
    if (is_kind(type, _ZTVN10__cxxabiv120__si_class_type_infoE)) {
        const auto *single = reinterpret_cast<const single_inheritance_information *>(type);
        return contains_source(single->parent, object, search);
    }
    if (is_kind(type, _ZTVN10__cxxabiv121__vmi_class_type_infoE)) {
        const auto *multiple = reinterpret_cast<const multiple_inheritance_information *>(type);
        for (unsigned int index = 0; index < multiple->base_count; index++) {
            const base_class_information &base = multiple->bases[index];
            if ((base.offset_flags & PUBLIC_BASE) && contains_source(base.type, object + base_offset(base, object), search)) {
                return true;
            }
        }
    }
    return false;
}

static void search_destination(const type_information *type, const char *object, bool is_public, cast_search &search) {
    if (same_type(type, search.destination_type)) {
        if (contains_source(type, object, search)) {
            remember(search.containing_source, object);
        }
        if (is_public) {
            remember(search.public_destinations, object);
        }
        return;
    }
    if (is_kind(type, _ZTVN10__cxxabiv120__si_class_type_infoE)) {
        const auto *single = reinterpret_cast<const single_inheritance_information *>(type);
        search_destination(single->parent, object, is_public, search);
        return;
    }
    if (is_kind(type, _ZTVN10__cxxabiv121__vmi_class_type_infoE)) {
        const auto *multiple = reinterpret_cast<const multiple_inheritance_information *>(type);
        for (unsigned int index = 0; index < multiple->base_count; index++) {
            const base_class_information &base = multiple->bases[index];
            search_destination(base.type, object + base_offset(base, object), is_public && (base.offset_flags & PUBLIC_BASE), search);
        }
    }
}

extern "C" void *__dynamic_cast(const void *source, const type_information *source_type, const type_information *destination_type,
                                ptrdiff_t source_to_destination) {
    if (!source) {
        return nullptr;
    }
    const char *const *vtable = *reinterpret_cast<const char *const *const *>(source);
    ptrdiff_t offset_to_top = reinterpret_cast<const ptrdiff_t *>(vtable)[-2];
    const auto *whole_type = reinterpret_cast<const type_information *const *>(vtable)[-1];
    const char *whole = static_cast<const char *>(source) + offset_to_top;
    if (!whole_type) {
        return nullptr;
    }
    cast_search search = {};
    search.destination_type = destination_type;
    search.source_type = source_type;
    search.source = static_cast<const char *>(source);
    if (same_type(whole_type, destination_type)) {
        return contains_source(whole_type, whole, search) ? const_cast<char *>(whole) : nullptr;
    }
    search_destination(whole_type, whole, true, search);
    if (search.containing_source.count == 1) {
        return const_cast<char *>(search.containing_source.objects[0]);
    }
    if (search.public_destinations.count == 1 && contains_source(whole_type, whole, search)) {
        return const_cast<char *>(search.public_destinations.objects[0]);
    }
    return nullptr;
}
