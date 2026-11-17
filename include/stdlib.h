#ifndef GRIMACE_STDLIB_H
#define GRIMACE_STDLIB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif

typedef void (*_invalid_parameter_handler)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, __UINTPTR_TYPE__);

/**
 * Converts a wide-character decimal string to a long.
 *
 * Skips leading white space, accepts an optional sign and stops at the first
 * character that isn't a digit.
 *
 * @param string The string to convert.
 * @return The value, or 0 if nothing could be converted.
 */
long _wtol(const wchar_t *string);
#define _wtoi _wtol
/**
 * Converts a wide-character decimal string to a long long.
 *
 * Skips leading white space, accepts an optional sign and stops at the first
 * character that isn't a digit.
 *
 * @param string The string to convert.
 * @return The value, or 0 if nothing could be converted.
 */
long long _wtoll(const wchar_t *string);
/**
 * Converts a multibyte string to a wide-character string.
 *
 * The result is only null-terminated if there's room for the terminator
 * within count.
 *
 * @param destination Output buffer, or NULL to get the length needed.
 * @param source Null-terminated multibyte string.
 * @param count Size of destination, in wide characters.
 * @return Wide characters written, not counting the terminator, or
 *         (size_t)-1 if source holds an invalid multibyte character.
 */
size_t mbstowcs(wchar_t *destination, const char *source, size_t count);
/**
 * Converts a wide-character string to a multibyte string.
 *
 * The result is only null-terminated if there's room for the terminator
 * within count.
 *
 * @param destination Output buffer, or NULL to get the length needed.
 * @param source Null-terminated wide-character string.
 * @param count Size of destination, in bytes.
 * @return Bytes written, not counting the terminator, or (size_t)-1 if a
 *         character can't be converted.
 */
size_t wcstombs(char *destination, const wchar_t *source, size_t count);

#define RAND_MAX 0x7FFF

typedef struct _div_t {
    int quot;
    int rem;
} div_t;

typedef struct _ldiv_t {
    long quot;
    long rem;
} ldiv_t;

typedef struct {
    long long quot;
    long long rem;
} lldiv_t;

/**
 * Allocates a suitably aligned heap block of at least size bytes.
 *
 * A size of zero may return a valid zero-length allocation.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to the block, or NULL if allocation fails.
 *
 * @note Windows CE 2.0 and later.
 */
void *malloc(size_t size);
/**
 * Allocates an array and initializes every byte to zero.
 *
 * @param count Number of elements.
 * @param size Size of each element in bytes.
 * @return Pointer to the allocated, suitably aligned block, or NULL if
 *         allocation fails.
 *
 * @note Windows CE 3.0 and later.
 */
void *calloc(size_t count, size_t size);
/**
 * Changes the size of a malloc or realloc block, preserving its contents.
 *
 * The block may move. If memory is NULL, realloc behaves like malloc. If
 * size is zero and memory is non-NULL, the block is freed and NULL returned.
 * On allocation failure, the original block is unchanged.
 *
 * @param memory Existing block, or NULL.
 * @param size New size in bytes.
 * @return Pointer to the resized block, or NULL on failure.
 *
 * @note Windows CE 2.0 and later.
 */
void *realloc(void *memory, size_t size);
/**
 * Frees a block allocated by malloc or realloc.
 *
 * Passing NULL is ignored. Passing any other invalid pointer has
 * undefined effects.
 *
 * @param memory Block to free, or NULL.
 *
 * @note Windows CE 2.0 and later.
 */
void free(void *memory);
/**
 * Returns the size in bytes of a heap block allocated by malloc or realloc.
 *
 * @param memory Pointer to the allocated block.
 * @return Block size in bytes.
 *
 * @note Windows CE 2.0 and later.
 */
size_t _msize(void *memory);
/**
 * Returns the absolute value of an integer.
 *
 * @param value Integer to convert.
 * @return Absolute value.
 *
 * @note Windows CE 2.0 and later.
 */
int abs(int value);
/**
 * Returns the absolute value of a long integer.
 *
 * @param value Long integer to convert.
 * @return Absolute value.
 *
 * @note Windows CE 2.0 and later.
 */
long labs(long value);
/**
 * Returns the integer quotient and remainder of dividend divided by divisor.
 *
 * The quotient is truncated toward zero. A zero divisor terminates the
 * program with an error message.
 *
 * @param dividend Numerator.
 * @param divisor Denominator.
 * @return div_t containing quot and rem.
 *
 * @note Windows CE 2.0 and later.
 */
div_t div(int dividend, int divisor);
/**
 * Returns the long-integer quotient and remainder of dividend divided by divisor.
 *
 * The quotient is truncated toward zero. A zero divisor terminates the
 * program with an error message.
 *
 * @param dividend Numerator.
 * @param divisor Denominator.
 * @return ldiv_t containing quot and rem.
 *
 * @note Windows CE 2.0 and later.
 */
ldiv_t ldiv(long dividend, long divisor);
/**
 * Converts a decimal integer string to int.
 *
 * Leading spaces and an optional sign are accepted. Parsing stops at the
 * first non-digit; decimal points and exponents are not recognized.
 *
 * @param string Null-terminated number string.
 * @return Converted value, or 0 if no conversion is possible. Overflow is
 *         undefined.
 *
 * @note Windows CE 2.0 and later.
 */
int atoi(const char *string);
/**
 * Converts a decimal integer string to long.
 *
 * Leading spaces and an optional sign are accepted. Parsing stops at the
 * first non-digit; decimal points and exponents are not recognized.
 *
 * @param string Null-terminated number string.
 * @return Converted value, or 0L if no conversion is possible. Overflow is
 *         undefined.
 *
 * @note Windows CE 2.0 and later.
 */
long atol(const char *string);
/**
 * Converts a numeric string to double precision.
 *
 * Parsing stops at the first character that is not part of the number.
 * The radix character follows the LC_NUMERIC locale setting; input is
 * limited to 100 characters.
 *
 * @param string Null-terminated number string.
 * @return Converted value, or 0.0 if it cannot be converted. Overflow is
 *         undefined.
 *
 * @note Windows CE 2.0 and later.
 */
double atof(const char *string);
/**
 * Converts a decimal integer string to a long long.
 *
 * Leading spaces and an optional sign are accepted. Parsing stops at the
 * first non-digit; decimal points and exponents are not recognized.
 *
 * @param string Null-terminated number string.
 * @return Converted value, or 0 if no conversion is possible. Overflow is
 *         undefined.
 *
 * @note Windows CE 3.0 and later.
 */
long long _atoi64(const char *string);
/**
 * Converts the initial numeric portion of string to double precision.
 *
 * The radix character follows LC_NUMERIC. end receives the first character
 * not consumed, or string if no conversion is possible.
 *
 * @param string Null-terminated number string.
 * @param end Receives the stopping character, or NULL if not needed.
 * @return Converted value, signed HUGE_VAL on overflow, or 0 if no
 *         conversion is possible or underflow occurs.
 *
 * @note Windows CE 3.0 and later.
 */
double strtod(const char *string, char **end);
/**
 * Converts the initial portion of string to a long integer in base.
 *
 * Parsing stops at the first invalid digit. end receives that position, or
 * string if no conversion is possible. base must be 0 or from 2 through 36.
 *
 * @param string Null-terminated number string.
 * @param end Receives the stopping character, or NULL if not needed.
 * @param base Number base; 0 selects it from the input prefix.
 * @return Converted value, LONG_MAX or LONG_MIN on overflow, or 0 if no
 *         conversion is possible.
 *
 * @note Windows CE 3.0 and later.
 */
long strtol(const char *string, char **end, int base);
/**
 * Converts the initial portion of string to an unsigned long in base.
 *
 * Parsing stops at the first invalid digit. end receives that position, or
 * string if no conversion is possible. base must be 0 or from 2 through 36.
 *
 * @param string Null-terminated number string.
 * @param end Receives the stopping character, or NULL if not needed.
 * @param base Number base; 0 selects it from the input prefix.
 * @return Converted value, or 0 if no conversion is possible.
 *
 * @note Windows CE 3.0 and later.
 */
unsigned long strtoul(const char *string, char **end, int base);
/**
 * Sorts count array elements in ascending order using quicksort.
 *
 * The comparator receives pointers to two elements and returns negative,
 * zero or positive when the first is less than, equal to or greater than
 * the second.
 *
 * @param base Start of the array.
 * @param count Number of elements.
 * @param size Size of each element in bytes.
 * @param compare Comparator for two elements.
 *
 * @note Windows CE 2.0 and later.
 */
void qsort(void *base, size_t count, size_t size, int (*compare)(const void *, const void *));
/**
 * Returns a pseudorandom integer from 0 through RAND_MAX.
 *
 * Call srand to seed the generator. Without a prior call, the sequence is
 * the same as with srand(1).
 *
 * @return Pseudorandom integer.
 *
 * @note Windows CE 2.0 and later.
 */
int rand(void);
/**
 * Sets the starting point for the rand pseudorandom sequence.
 *
 * Calling srand(1) restarts the default sequence.
 *
 * @param seed Seed for the generator.
 *
 * @note Windows CE 2.0 and later.
 */
void srand(unsigned int seed);
/**
 * Converts an integer to a null-terminated string in the specified base.
 *
 * radix must be from 2 through 36. A minus sign is emitted for negative
 * values only in base 10.
 *
 * @param value Integer to convert.
 * @param buffer Receives the result.
 * @param radix Number base.
 * @return buffer.
 *
 * @note Windows CE 2.0 and later.
 */
char *_itoa(int value, char *buffer, int radix);
/**
 * Converts a long integer to a null-terminated string in the specified base.
 *
 * radix must be from 2 through 36. A minus sign is emitted for negative
 * values only in base 10.
 *
 * @param value Long integer to convert.
 * @param buffer Receives the result.
 * @param radix Number base.
 * @return buffer.
 *
 * @note Windows CE 2.0 and later.
 */
char *_ltoa(long value, char *buffer, int radix);
/**
 * Converts an unsigned long to a null-terminated string in the specified base.
 *
 * radix must be from 2 through 36. No overflow checking is performed.
 *
 * @param value Unsigned long to convert.
 * @param buffer Receives the result.
 * @param radix Number base.
 * @return buffer.
 *
 * @note Windows CE 2.0 and later.
 */
char *_ultoa(unsigned long value, char *buffer, int radix);
/**
 * Converts value to a string containing digits significant digits.
 *
 * The output contains digits only. decimal receives the decimal-point
 * position; sign is 0 for positive and nonzero for negative. The result
 * uses a shared static buffer overwritten by the next _ecvt or _fcvt call.
 *
 * @param value Number to convert.
 * @param digits Number of significant digits.
 * @param decimal Receives the decimal-point position.
 * @param sign Receives the sign indicator.
 * @return Pointer to the digit string.
 *
 * @note Windows CE 2.0 and later.
 */
char *_ecvt(double value, int digits, int *decimal, int *sign);
/**
 * Converts value to a digit string with digits digits after the decimal.
 *
 * The output contains digits only. decimal receives the decimal-point
 * position; sign is 0 for positive and nonzero for negative. The result
 * uses a shared static buffer overwritten by the next _ecvt or _fcvt call.
 *
 * @param value Number to convert.
 * @param digits Number of digits after the decimal point.
 * @param decimal Receives the decimal-point position.
 * @param sign Receives the sign indicator.
 * @return Pointer to the digit string.
 *
 * @note Windows CE 2.0 and later.
 */
char *_fcvt(double value, int digits, int *decimal, int *sign);
/**
 * Converts value to a decimal or exponential string in buffer.
 *
 * The output may include a sign, decimal point and exponent. Provide room
 * beyond digits plus one byte; the function does not check buffer size.
 *
 * @param value Number to convert.
 * @param digits Number of significant digits to produce.
 * @param buffer Receives the null-terminated result.
 * @return buffer.
 *
 * @note Windows CE 2.0 and later.
 */
char *_gcvt(double value, int digits, char *buffer);
/**
 * Rotates the bits of value left, wrapping shifted-out bits to the right.
 *
 * @param value Value to rotate.
 * @param shift Number of bit positions.
 * @return Rotated value.
 *
 * @note Windows CE 2.0 and later.
 */
unsigned int _rotl(unsigned int value, int shift);
/**
 * Rotates the bits of value right, wrapping shifted-out bits to the left.
 *
 * @param value Value to rotate.
 * @param shift Number of bit positions.
 * @return Rotated value.
 *
 * @note Windows CE 2.0 and later.
 */
unsigned int _rotr(unsigned int value, int shift);
/**
 * Rotates the bits of value left, wrapping shifted-out bits to the right.
 *
 * @param value Value to rotate.
 * @param shift Number of bit positions.
 * @return Rotated value.
 *
 * @note Windows CE 2.0 and later.
 */
unsigned long _lrotl(unsigned long value, int shift);
/**
 * Rotates the bits of value right, wrapping shifted-out bits to the left.
 *
 * @param value Value to rotate.
 * @param shift Number of bit positions.
 * @return Rotated value.
 *
 * @note Windows CE 2.0 and later.
 */
unsigned long _lrotr(unsigned long value, int shift);
/**
 * Copies count bytes from source to destination, swapping adjacent bytes.
 *
 * Use an even count so every copied byte has a pair.
 *
 * @param source Bytes to copy and swap.
 * @param destination Receives the swapped bytes.
 * @param count Number of bytes to process.
 *
 * @note Windows CE 2.0 and later.
 */
void _swab(char *source, char *destination, int count);
long long llabs(long long value);
lldiv_t lldiv(long long dividend, long long divisor);
long long strtoll(const char *string, char **end, int base);
unsigned long long strtoull(const char *string, char **end, int base);
long long atoll(const char *string);
float strtof(const char *string, char **end);
long double strtold(const char *string, char **end);
__attribute__((noreturn)) void abort(void);
__attribute__((noreturn)) void _exit(int status);
__attribute__((noreturn)) void _Exit(int status);
__attribute__((noreturn)) void exit(int status);
int atexit(void (*handler)(void));
char *getenv(const char *name);
void *bsearch(const void *key, const void *base, size_t count, size_t size, int (*compare)(const void *, const void *));
int system(const char *command);
#ifdef GRIMACE_POSIX
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);
int putenv(char *assignment);
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

/**
 * Returns the absolute value of a 64-bit integer.
 *
 * Not in Microsoft's Windows CE reference; signature from the Visual C++ CRT
 * reference. Windows CE 4.1 and later.
 *
 * @param value Integer value.
 * @return Absolute value of value.
 */
long long _abs64(long long value);
/**
 * Reverses the byte order of a 64-bit value.
 *
 * Converts between big-endian and little-endian representations. Windows CE
 * 4.1 and later.
 *
 * @param value Value to swap.
 * @return value with its bytes reversed.
 */
unsigned long long _byteswap_uint64(unsigned long long value);
/**
 * Reverses the byte order of a 32-bit value.
 *
 * Converts between big-endian and little-endian representations. Windows CE
 * 4.1 and later.
 *
 * @param value Value to swap.
 * @return value with its bytes reversed.
 */
unsigned long _byteswap_ulong(unsigned long value);
/**
 * Reverses the byte order of a 16-bit value.
 *
 * Converts between big-endian and little-endian representations. Windows CE
 * 4.1 and later.
 *
 * @param value Value to swap.
 * @return value with its bytes reversed.
 */
unsigned short _byteswap_ushort(unsigned short value);
/**
 * Gets and sets the floating-point control word.
 *
 * Bits set in mask are replaced by those in value. A mask of 0 just reads the
 * control word. Unlike _controlfp, invalid bits are reported as an error.
 * Windows CE 6.0 and later.
 *
 * @param current Receives the control word after the change. Can be NULL.
 * @param value New control word bits.
 * @param mask Bits to change.
 * @return 0 on success, or EINVAL for an invalid mask or value.
 */
errno_t _controlfp_s(unsigned int *current, unsigned int value, unsigned int mask);
/**
 * Converts a double to a string of digits, rounded to a number of significant
 * digits.
 *
 * The string holds only digits; the decimal point position and sign come back
 * separately. Bad arguments call the invalid parameter handler (see
 * _set_invalid_parameter_handler); if it returns, the function sets errno and
 * returns the error. Windows CE 6.0 and later.
 *
 * @param buffer Receives the digits.
 * @param size Size of buffer in bytes.
 * @param value Number to convert.
 * @param digits Number of significant digits.
 * @param decimal Receives the position of the decimal point relative to the
 *        start of the string.
 * @param sign Receives nonzero if value is negative.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _ecvt_s(char *buffer, size_t size, double value, int digits, int *decimal, int *sign);
/**
 * Converts a double to a string of digits, rounded to a number of digits after
 * the decimal point.
 *
 * The string holds only digits; the decimal point position and sign come back
 * separately. Bad arguments call the invalid parameter handler (see
 * _set_invalid_parameter_handler); if it returns, the function sets errno and
 * returns the error. Windows CE 6.0 and later.
 *
 * @param buffer Receives the digits.
 * @param size Size of buffer in bytes.
 * @param value Number to convert.
 * @param digits Number of digits after the decimal point.
 * @param decimal Receives the position of the decimal point relative to the
 *        start of the string.
 * @param sign Receives nonzero if value is negative.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _fcvt_s(char *buffer, size_t size, double value, int digits, int *decimal, int *sign);
/**
 * Converts a double to a string with a number of significant digits, in fixed
 * or exponential form.
 *
 * Bad arguments call the invalid parameter handler (see
 * _set_invalid_parameter_handler); if it returns, the function sets errno and
 * returns the error. Windows CE 6.0 and later.
 *
 * @param buffer Receives the string.
 * @param size Size of buffer in bytes.
 * @param value Number to convert.
 * @param digits Number of significant digits.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _gcvt_s(char *buffer, size_t size, double value, int digits);
/**
 * Gets the function the CRT calls when a function detects an invalid argument.
 *
 * Not listed in Microsoft's CE 6.0 table of security-enhanced functions, but
 * exported by coredll. Windows CE 6.0 and later.
 *
 * @return The current handler, or NULL if none has been set.
 */
_invalid_parameter_handler _get_invalid_parameter_handler(void);
/**
 * Reads a line from stdin into a buffer of known size.
 *
 * The newline is dropped. If the line doesn't fit, buffer[0] is set to 0 and
 * the invalid parameter handler is called. Windows CE 6.0 and later.
 *
 * @param buffer Receives the line.
 * @param size Size of buffer in characters, including the terminator.
 * @return buffer, or NULL at end of file or on error.
 */
wchar_t *_getws_s(wchar_t *buffer, size_t size);
/**
 * Converts an int to a string in a buffer of known size.
 *
 * The buffer must hold every digit, any sign and the terminator. Bad arguments
 * call the invalid parameter handler (see _set_invalid_parameter_handler); if
 * it returns, the function sets errno and returns the error. Windows CE 6.0
 * and later.
 *
 * @param value Number to convert.
 * @param buffer Receives the string.
 * @param size Size of buffer in characters.
 * @param radix Base, 2 to 36.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _itoa_s(int value, char *buffer, size_t size, int radix);
/**
 * Converts an int to a string in a buffer of known size.
 *
 * The buffer must hold every digit, any sign and the terminator. Bad arguments
 * call the invalid parameter handler (see _set_invalid_parameter_handler); if
 * it returns, the function sets errno and returns the error. Windows CE 6.0
 * and later.
 *
 * @param value Number to convert.
 * @param buffer Receives the string.
 * @param size Size of buffer in characters.
 * @param radix Base, 2 to 36.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _itow_s(int value, wchar_t *buffer, size_t size, int radix);
/**
 * Converts a long to a string in a buffer of known size.
 *
 * The buffer must hold every digit, any sign and the terminator. Bad arguments
 * call the invalid parameter handler (see _set_invalid_parameter_handler); if
 * it returns, the function sets errno and returns the error. Windows CE 6.0
 * and later.
 *
 * @param value Number to convert.
 * @param buffer Receives the string.
 * @param size Size of buffer in characters.
 * @param radix Base, 2 to 36.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _ltoa_s(long value, char *buffer, size_t size, int radix);
/**
 * Converts a long to a string in a buffer of known size.
 *
 * The buffer must hold every digit, any sign and the terminator. Bad arguments
 * call the invalid parameter handler (see _set_invalid_parameter_handler); if
 * it returns, the function sets errno and returns the error. Windows CE 6.0
 * and later.
 *
 * @param value Number to convert.
 * @param buffer Receives the string.
 * @param size Size of buffer in characters.
 * @param radix Base, 2 to 36.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _ltow_s(long value, wchar_t *buffer, size_t size, int radix);
/**
 * Resizes a block from calloc to count elements of size bytes, zeroing any
 * added space.
 *
 * Like realloc, the block may move and the old pointer is invalid on success.
 * On failure the original block is untouched. Windows CE 6.0 and later.
 *
 * @param memory Block from calloc or _recalloc, or NULL to allocate a new one.
 * @param count Number of elements.
 * @param size Size of each element in bytes.
 * @return Pointer to the resized block, or NULL on failure.
 */
void *_recalloc(void *memory, size_t count, size_t size);
/**
 * Rotates the bits of a 64-bit value to the left.
 *
 * Bits shifted off one end come back in at the other. Not in Microsoft's
 * Windows CE reference; signature from the Visual C++ CRT reference. Windows
 * CE 4.1 and later.
 *
 * @param value Value to rotate.
 * @param shift Number of bits to rotate by.
 * @return The rotated value.
 */
unsigned long long _rotl64(unsigned long long value, int shift);
/**
 * Rotates the bits of a 64-bit value to the right.
 *
 * Bits shifted off one end come back in at the other. Not in Microsoft's
 * Windows CE reference; signature from the Visual C++ CRT reference. Windows
 * CE 4.1 and later.
 *
 * @param value Value to rotate.
 * @param shift Number of bits to rotate by.
 * @return The rotated value.
 */
unsigned long long _rotr64(unsigned long long value, int shift);
/**
 * Sets the function the CRT calls when a function detects an invalid argument.
 *
 * The handler receives the expression, function name, file and line (these may
 * be NULL or 0 in release builds). If it returns, the failing function sets
 * errno and returns an error code. Windows CE 6.0 and later.
 *
 * @param handler New handler.
 * @return The previous handler.
 */
_invalid_parameter_handler _set_invalid_parameter_handler(_invalid_parameter_handler handler);
/**
 * Converts an unsigned long to a string in a buffer of known size.
 *
 * The buffer must hold every digit, any sign and the terminator. Bad arguments
 * call the invalid parameter handler (see _set_invalid_parameter_handler); if
 * it returns, the function sets errno and returns the error. Windows CE 6.0
 * and later.
 *
 * @param value Number to convert.
 * @param buffer Receives the string.
 * @param size Size of buffer in characters.
 * @param radix Base, 2 to 36.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _ultoa_s(unsigned long value, char *buffer, size_t size, int radix);
/**
 * Converts an unsigned long to a string in a buffer of known size.
 *
 * The buffer must hold every digit, any sign and the terminator. Bad arguments
 * call the invalid parameter handler (see _set_invalid_parameter_handler); if
 * it returns, the function sets errno and returns the error. Windows CE 6.0
 * and later.
 *
 * @param value Number to convert.
 * @param buffer Receives the string.
 * @param size Size of buffer in characters.
 * @param radix Base, 2 to 36.
 * @return 0 on success, otherwise an errno value such as EINVAL or ERANGE.
 */
errno_t _ultow_s(unsigned long value, wchar_t *buffer, size_t size, int radix);
/**
 * Converts a multibyte string to a wide string in a buffer of known size.
 *
 * The result is always null-terminated. Pass _TRUNCATE as count to copy as
 * much as fits and return STRUNCATE instead of failing. Pass NULL as
 * destination and 0 as size to get the size needed. Bad arguments call the
 * invalid parameter handler (see _set_invalid_parameter_handler); if it
 * returns, the function sets errno and returns the error. Windows CE 6.0 and
 * later.
 *
 * @param converted Receives the number of wide characters written, including
 *        the terminator. Can be NULL.
 * @param destination Receives the wide string, or NULL.
 * @param size Size of destination in wide characters.
 * @param source Multibyte string to convert.
 * @param count Maximum number of wide characters to store, excluding the
 *        terminator, or _TRUNCATE.
 * @return 0 on success, STRUNCATE if truncated with _TRUNCATE, otherwise an
 *         errno value such as EINVAL, ERANGE or EILSEQ.
 */
errno_t mbstowcs_s(size_t *converted, wchar_t *destination, size_t size, const char *source, size_t count);
/**
 * Sorts an array, passing a caller-supplied context to the comparison
 * function.
 *
 * compare receives context first, then pointers to the two elements, and
 * returns less than, equal to or greater than zero. Windows CE 6.0 and later.
 *
 * @param base Start of the array.
 * @param count Number of elements.
 * @param size Size of each element in bytes.
 * @param compare Comparison function.
 * @param context Value passed through to compare.
 */
void qsort_s(void *base, size_t count, size_t size, int (*compare)(void *, const void *, const void *), void *context);
/**
 * Generates a random number without needing a seed.
 *
 * Unlike rand the sequence is not reproducible. The image must include
 * filesys.dll (any of the SYSGEN_FS* variables, SYSGEN_ACL or
 * SYSGEN_ACCOUNTDB). Windows CE 6.0 and later.
 *
 * @param value Receives a number from 0 to UINT_MAX.
 * @return 0 on success, otherwise an errno value.
 */
errno_t rand_s(unsigned int *value);
/**
 * Converts a wide string to a multibyte string in a buffer of known size.
 *
 * The result is always null-terminated. Pass _TRUNCATE as count to copy as
 * much as fits and return STRUNCATE instead of failing. Pass NULL as
 * destination and 0 as size to get the size needed. Bad arguments call the
 * invalid parameter handler (see _set_invalid_parameter_handler); if it
 * returns, the function sets errno and returns the error. Windows CE 6.0 and
 * later.
 *
 * @param converted Receives the number of bytes written, including the
 *        terminator. Can be NULL.
 * @param destination Receives the multibyte string, or NULL.
 * @param size Size of destination in bytes.
 * @param source Wide string to convert.
 * @param count Maximum number of bytes to store, excluding the terminator, or
 *        _TRUNCATE.
 * @return 0 on success, STRUNCATE if truncated with _TRUNCATE, otherwise an
 *         errno value such as EINVAL, ERANGE or EILSEQ.
 */
errno_t wcstombs_s(size_t *converted, char *destination, size_t size, const wchar_t *source, size_t count);

#ifdef __cplusplus
}
#endif

#ifdef GRIMACE_POSIX
#include <alloca.h>
#endif

#endif
