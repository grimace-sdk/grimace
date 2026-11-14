#ifndef GRIMACE_LIBCXX_LOCALE_BASE_API_H
#define GRIMACE_LIBCXX_LOCALE_BASE_API_H

#include <__config>

#if !_LIBCPP_HAS_LOCALIZATION
#  include_next <__locale_dir/locale_base_api.h>
#else

#  include <__cstddef/size_t.h>
#  include <__std_mbstate_t.h>
#  include <__utility/forward.h>
#  include <clocale>
#  include <cstdarg>
#  include <cstdio>
#  include <cstdlib>
#  include <cwchar>
#  include <errno.h>
#  include <wctype.h>

#  if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#    pragma GCC system_header
#  endif

struct __grimace_locale {
    int category_mask;
};

_LIBCPP_BEGIN_NAMESPACE_STD
namespace __locale {

#  define _LIBCPP_COLLATE_MASK 1
#  define _LIBCPP_CTYPE_MASK 2
#  define _LIBCPP_MONETARY_MASK 4
#  define _LIBCPP_NUMERIC_MASK 8
#  define _LIBCPP_TIME_MASK 16
#  define _LIBCPP_MESSAGES_MASK 32
#  define _LIBCPP_ALL_MASK 63
#  define _LIBCPP_LC_ALL LC_ALL

using __locale_t _LIBCPP_NODEBUG = ::__grimace_locale*;

inline _LIBCPP_HIDE_FROM_ABI bool __is_c_locale_name(const char* __name) {
    return !__name || !__name[0] || (__name[0] == 'C' && (!__name[1] || __name[1] == '.')) ||
           (__name[0] == 'P' && __name[1] == 'O' && __name[2] == 'S' && __name[3] == 'I' && __name[4] == 'X' && !__name[5]);
}

#  if defined(_LIBCPP_BUILDING_LIBRARY)
using __lconv_t _LIBCPP_NODEBUG = std::lconv;

inline _LIBCPP_HIDE_FROM_ABI __locale_t __newlocale(int __category_mask, const char* __name, __locale_t) {
    static ::__grimace_locale __c_locale = {_LIBCPP_ALL_MASK};
    if (!__is_c_locale_name(__name)) {
        errno = ENOENT;
        return nullptr;
    }
    return &__c_locale;
}

inline _LIBCPP_HIDE_FROM_ABI void __freelocale(__locale_t) {}

inline _LIBCPP_HIDE_FROM_ABI char* __setlocale(int __category, char const* __locale) {
    return ::setlocale(__category, __locale);
}

inline _LIBCPP_HIDE_FROM_ABI __lconv_t* __localeconv(__locale_t&) { return ::localeconv(); }

inline _LIBCPP_HIDE_FROM_ABI const char* __get_locale_encoding(__locale_t) { return "ISO-8859-1"; }

inline _LIBCPP_HIDE_FROM_ABI int __mb_len_max(__locale_t) { return 1; }

#    if _LIBCPP_HAS_WIDE_CHARACTERS
inline _LIBCPP_HIDE_FROM_ABI wint_t __btowc(int __character, __locale_t) {
    return __character == EOF ? WEOF : static_cast<wint_t>(static_cast<unsigned char>(__character));
}

inline _LIBCPP_HIDE_FROM_ABI int __wctob(wint_t __character, __locale_t) {
    return __character < 256 ? static_cast<int>(__character) : EOF;
}

inline _LIBCPP_HIDE_FROM_ABI size_t __wcrtomb(char* __output, wchar_t __character, mbstate_t*, __locale_t) {
    if (!__output) {
        return 1;
    }
    if (static_cast<unsigned>(__character) > 255) {
        errno = EILSEQ;
        return static_cast<size_t>(-1);
    }
    *__output = static_cast<char>(__character);
    return 1;
}

inline _LIBCPP_HIDE_FROM_ABI size_t __mbrtowc(wchar_t* __output, const char* __input, size_t __count, mbstate_t*, __locale_t) {
    if (!__input) {
        return 0;
    }
    if (__count == 0) {
        return static_cast<size_t>(-2);
    }
    wchar_t __character = static_cast<unsigned char>(*__input);
    if (__output) {
        *__output = __character;
    }
    return __character ? 1 : 0;
}

inline _LIBCPP_HIDE_FROM_ABI int __mbtowc(wchar_t* __output, const char* __input, size_t __count, __locale_t __locale) {
    if (!__input) {
        return 0;
    }
    return static_cast<int>(__mbrtowc(__output, __input, __count, nullptr, __locale));
}

inline _LIBCPP_HIDE_FROM_ABI size_t __mbrlen(const char* __input, size_t __count, mbstate_t*, __locale_t __locale) {
    return __mbrtowc(nullptr, __input, __count, nullptr, __locale);
}

inline _LIBCPP_HIDE_FROM_ABI size_t
__mbsnrtowcs(wchar_t* __output, const char** __input, size_t __input_count, size_t __output_count, mbstate_t*, __locale_t) {
    const char* __position = *__input;
    size_t __converted = 0;
    while (__input_count && (!__output || __converted < __output_count)) {
        wchar_t __character = static_cast<unsigned char>(*__position);
        if (__output) {
            __output[__converted] = __character;
        }
        if (!__character) {
            if (__output) {
                *__input = nullptr;
            }
            return __converted;
        }
        __converted++;
        __position++;
        __input_count--;
    }
    if (__output) {
        *__input = __position;
    }
    return __converted;
}

inline _LIBCPP_HIDE_FROM_ABI size_t
__mbsrtowcs(wchar_t* __output, const char** __input, size_t __output_count, mbstate_t* __state, __locale_t __locale) {
    return __mbsnrtowcs(__output, __input, static_cast<size_t>(-1), __output_count, __state, __locale);
}

inline _LIBCPP_HIDE_FROM_ABI size_t
__wcsnrtombs(char* __output, const wchar_t** __input, size_t __input_count, size_t __output_count, mbstate_t*, __locale_t) {
    const wchar_t* __position = *__input;
    size_t __converted = 0;
    while (__input_count && (!__output || __converted < __output_count)) {
        wchar_t __character = *__position;
        if (static_cast<unsigned>(__character) > 255) {
            errno = EILSEQ;
            if (__output) {
                *__input = __position;
            }
            return static_cast<size_t>(-1);
        }
        if (__output) {
            __output[__converted] = static_cast<char>(__character);
        }
        if (!__character) {
            if (__output) {
                *__input = nullptr;
            }
            return __converted;
        }
        __converted++;
        __position++;
        __input_count--;
    }
    if (__output) {
        *__input = __position;
    }
    return __converted;
}
#    endif
#  endif

template <class... _Args>
_LIBCPP_HIDE_FROM_ABI int __snprintf(char* __output, size_t __size, __locale_t, const char* __format, _Args&&... __arguments) {
    return std::snprintf(__output, __size, __format, std::forward<_Args>(__arguments)...);
}

template <class... _Args>
_LIBCPP_HIDE_FROM_ABI int __asprintf(char** __output, __locale_t, const char* __format, _Args&&... __arguments) {
    int __length = std::snprintf(nullptr, 0, __format, std::forward<_Args>(__arguments)...);
    if (__length < 0) {
        return -1;
    }
    *__output = static_cast<char*>(std::malloc(static_cast<size_t>(__length) + 1));
    if (!*__output) {
        return -1;
    }
    return std::snprintf(*__output, static_cast<size_t>(__length) + 1, __format, std::forward<_Args>(__arguments)...);
}

} // namespace __locale
_LIBCPP_END_NAMESPACE_STD

#  include <__locale_dir/support/default/get_c_locale.h>
#  include <__locale_dir/support/no_locale/characters.h>
#  include <__locale_dir/support/no_locale/strtonum.h>

#  define _LIBCPP_PROVIDES_DEFAULT_RUNE_TABLE 1

#endif

#endif
