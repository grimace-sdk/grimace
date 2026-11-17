#ifndef GRIMACE_STDIO_H
#define GRIMACE_STDIO_H

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif

#ifndef _FILE_DEFINED
typedef void FILE;
#define _FILE_DEFINED
#endif

typedef long long fpos_t;

#ifndef GRIMACE_WINT_T
#define GRIMACE_WINT_T
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
#endif

#define EOF (-1)
#define BUFSIZ 512
#define _IOFBF 0x0000
#define _IOLBF 0x0040
#define _IONBF 0x0004
#define WEOF ((wint_t)0xFFFF)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/**
 * Returns the standard stream identified by index.
 *
 * Index 0 is stdin, 1 is stdout, and 2 is stderr.
 *
 * @param index Standard stream index: 0, 1 or 2.
 * @return The corresponding FILE pointer.
 *
 * @note Windows CE 3.0 and later.
 */
FILE *_getstdfilex(int index);
#define stdin _getstdfilex(0)
#define stdout _getstdfilex(1)
#define stderr _getstdfilex(2)

/**
 * Opens a file using narrow-character path and mode strings.
 *
 * @param path File name.
 * @param mode Access mode.
 * @return The open stream, or NULL on error.
 *
 * @note Windows CE 2.0 and later.
 */
FILE *fopen(const char *path, const char *mode);
/**
 * Opens a file using wide-character path and mode strings.
 *
 * @param path Wide-character file name.
 * @param mode Wide-character access mode.
 * @return The open stream, or NULL on error.
 *
 * @note Windows CE 3.0 and later.
 */
FILE *_wfopen(const wchar_t *path, const wchar_t *mode);
/**
 * Associates a wide-mode stream with a file opened for low-level I/O.
 *
 * @param handle Handle of the already-open file.
 * @param mode Wide-character access mode.
 * @return The new stream, or NULL on error.
 *
 * @note Windows CE 3.0 and later.
 */
FILE *_wfdopen(void *handle, const wchar_t *mode);
/**
 * Closes a stream and reassigns it to a file using wide-character strings.
 *
 * If reopening fails, the original stream is still closed.
 *
 * @param path Wide-character file name.
 * @param mode Wide-character access mode.
 * @param stream Stream to close and reassign.
 * @return The reopened stream, or NULL on error.
 *
 * @note Windows CE 3.0 and later.
 */
FILE *_wfreopen(const wchar_t *path, const wchar_t *mode, FILE *stream);
/**
 * Flushes and closes a stream.
 *
 * @param stream Stream to close.
 * @return 0 on success, or EOF on error.
 *
 * @note Windows CE 2.0 and later.
 */
int fclose(FILE *stream);
/**
 * Closes all open streams except stdin, stdout and stderr.
 *
 * It also closes and deletes temporary files created by tmpfile. Stream
 * buffers are flushed before closing.
 *
 * @return Number of streams closed, or EOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
int _fcloseall(void);
/**
 * Reads up to count items of size bytes from a stream.
 *
 * The return value counts full items only. If size or count is zero, no
 * data is read and buffer is unchanged.
 *
 * @param buffer Receives the data.
 * @param size Size of each item in bytes.
 * @param count Maximum number of items to read.
 * @param stream Stream to read.
 * @return Number of full items read; use feof or ferror to identify why
 *         fewer than count items were read.
 *
 * @note Windows CE 2.0 and later.
 */
size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
/**
 * Writes up to count items of size bytes to a stream.
 *
 * In text mode, carriage returns are translated to carriage-return and
 * line-feed pairs.
 *
 * @param buffer Data to write.
 * @param size Size of each item in bytes.
 * @param count Maximum number of items to write.
 * @param stream Destination stream.
 * @return Number of full items written; the position is indeterminate if
 *         an error occurs.
 *
 * @note Windows CE 2.0 and later.
 */
size_t fwrite(const void *buffer, size_t size, size_t count, FILE *stream);
/**
 * Reads one character from a stream and advances its position.
 *
 * Use feof or ferror to distinguish end of file from a read error.
 *
 * @param stream Stream to read.
 * @return The character as an int, or EOF on error or end of file.
 *
 * @note Windows CE 3.0 and later.
 */
int fgetc(FILE *stream);
/**
 * Writes one character to a stream.
 *
 * @param character Character to write.
 * @param stream Destination stream.
 * @return The character written, or EOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
int fputc(int character, FILE *stream);
/**
 * Pushes one character back onto a stream and clears its end-of-file flag.
 *
 * The stream must be open for reading. A later read returns the pushed
 * character first. Do not push EOF; only one character of pushback is
 * reliable.
 *
 * @param character Character to push back.
 * @param stream Stream to update.
 * @return The character on success, or EOF if pushback fails.
 *
 * @note Windows CE 3.0 and later.
 */
int ungetc(int character, FILE *stream);
/**
 * Reads at most size - 1 characters from a stream into buffer.
 *
 * Reading stops at a newline, end of file, or the size limit. A newline is
 * retained, and the result is null-terminated.
 *
 * @param buffer Receives the string.
 * @param size Buffer capacity in characters.
 * @param stream Stream to read.
 * @return buffer on success, or NULL on error or end of file.
 *
 * @note Windows CE 3.0 and later.
 */
char *fgets(char *buffer, int size, FILE *stream);
/**
 * Writes a string to a stream without its terminating null character.
 *
 * @param string Null-terminated string to write.
 * @param stream Destination stream.
 * @return A nonnegative value on success, or EOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
int fputs(const char *string, FILE *stream);
/**
 * Moves a stream's position indicator relative to origin.
 *
 * origin is SEEK_SET, SEEK_CUR or SEEK_END. In text mode, only zero offsets
 * or offsets returned by ftell from the start of the file are guaranteed.
 * The call clears the end-of-file indicator and cancels ungetc effects.
 *
 * @param stream Stream to reposition.
 * @param offset Offset from origin.
 * @param origin SEEK_SET, SEEK_CUR or SEEK_END.
 * @return 0 on success, nonzero on failure; the result is undefined on
 *         devices that cannot seek.
 *
 * @note Windows CE 2.0 and later.
 */
int fseek(FILE *stream, long offset, int origin);
/**
 * Returns a stream's current position relative to its start.
 *
 * In text mode, the result may not be a physical byte offset. Use it with
 * fseek to restore positions. Its value is undefined on nonseekable devices.
 *
 * @param stream Stream to query.
 * @return Current position, or -1L on error.
 *
 * @note Windows CE 3.0 and later.
 */
long ftell(FILE *stream);
/**
 * Saves the current position indicator of a stream.
 *
 * The saved value is for use with fsetpos on the same stream.
 *
 * @param stream Stream whose position is saved.
 * @param position Receives the position indicator.
 * @return 0 on success, or a nonzero value on failure.
 *
 * @note Windows CE 3.0 and later.
 */
int fgetpos(FILE *stream, fpos_t *position);
/**
 * Restores a stream position saved by fgetpos.
 *
 * The call clears the end-of-file indicator and cancels ungetc effects.
 *
 * @param stream Stream to reposition.
 * @param position Saved position from fgetpos.
 * @return 0 on success, or a nonzero value on failure.
 *
 * @note Windows CE 3.0 and later.
 */
int fsetpos(FILE *stream, const fpos_t *position);
/**
 * Flushes a stream's output buffer or clears its input buffer.
 *
 * Pass NULL to flush all open output streams. The stream remains open.
 *
 * @param stream Stream to flush or clear, or NULL for all output streams.
 * @return 0 on success, or EOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
int fflush(FILE *stream);
/**
 * Flushes output streams and clears input-stream buffers.
 *
 * All streams remain open.
 *
 * @return Number of open streams.
 *
 * @note Windows CE 3.0 and later.
 */
int _flushall(void);
/**
 * Tests whether a read has attempted to pass the end of a stream.
 *
 * @param stream Stream to test.
 * @return Nonzero if the end-of-file indicator is set; otherwise 0.
 *
 * @note Windows CE 3.0 and later.
 */
int feof(FILE *stream);
/**
 * Tests whether a stream's error indicator is set.
 *
 * @param stream Stream to test.
 * @return Nonzero if an error occurred; otherwise 0.
 *
 * @note Windows CE 3.0 and later.
 */
int ferror(FILE *stream);
/**
 * Clears a stream's error and end-of-file indicators.
 *
 * @param stream Stream to clear.
 *
 * @note Windows CE 3.0 and later.
 */
void clearerr(FILE *stream);
/**
 * Returns the file handle associated with a stream.
 *
 * The result is undefined if stream is not associated with an open file.
 *
 * @param stream Stream whose file handle is requested.
 * @return The associated file handle.
 *
 * @note Windows CE 3.0 and later.
 */
void *_fileno(FILE *stream);
/**
 * Sets the text or binary translation mode for a stream.
 *
 * Use _O_TEXT for CR-LF translation or _O_BINARY to suppress it. Set the
 * mode before doing input or output on the stream.
 *
 * @param stream Stream whose translation mode changes.
 * @param mode _O_TEXT or _O_BINARY.
 * @return The previous mode, or -1 on error.
 *
 * @note Windows CE 3.0 and later.
 */
int _setmode(FILE *stream, int mode);
/**
 * Reads one character from stdin.
 *
 * @return The character as an int, or EOF on error or end of file.
 *
 * @note Windows CE 3.0 and later.
 */
int getchar(void);
/**
 * Writes one character to stdout.
 *
 * @param character Character to write.
 * @return The character written, or EOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
int putchar(int character);
/**
 * Reads a line from stdin, replacing the newline with a null terminator.
 *
 * The declaration has no buffer-size argument, so the caller must provide
 * enough storage.
 *
 * @param buffer Receives the input line.
 * @return buffer on success, or NULL on error or end of file.
 *
 * @note Windows CE 3.0 and later.
 */
char *gets(char *buffer);
/**
 * Writes a string to stdout and appends a newline.
 *
 * @param string Null-terminated string to write.
 * @return A nonnegative value on success, or EOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
int puts(const char *string);

/**
 * Writes formatted output to stdout.
 *
 * The format string controls how each optional argument is converted.
 *
 * @param format Format-control string.
 * @return Number of characters printed, or a negative value on error.
 *
 * @note Windows CE 3.0 and later.
 */
int printf(const char *format, ...);
/**
 * Writes formatted output to a stream.
 *
 * The format string controls how each optional argument is converted.
 *
 * @param stream Destination stream.
 * @param format Format-control string.
 * @return Number of bytes written, or a negative value on output error.
 *
 * @note Windows CE 3.0 and later.
 */
int fprintf(FILE *stream, const char *format, ...);
/**
 * Formats output into buffer and appends a null terminator.
 *
 * The buffer must be large enough for the formatted output and terminator.
 *
 * @param buffer Receives the formatted string.
 * @param format Format-control string.
 * @return Number of characters written, excluding the null terminator.
 *
 * @note Windows CE 2.0 and later.
 */
int sprintf(char *buffer, const char *format, ...);
/**
 * Formats output into a buffer, storing no more than count bytes.
 *
 * The terminating null is appended unless count is zero or the formatted
 * length is at least count. A truncated result returns a negative value.
 *
 * @param buffer Receives the formatted output.
 * @param count Maximum number of bytes to store.
 * @param format Format-control string.
 * @return Bytes stored excluding the null terminator, or a negative value
 *         if the output does not fit.
 *
 * @note Windows CE 2.0 and later.
 */
int _snprintf(char *buffer, size_t count, const char *format, ...);
/**
 * Writes formatted output to stdout using a va_list.
 *
 * Initialize arguments with va_start. This function does not call va_end.
 *
 * @param format Format-control string.
 * @param arguments Argument list initialized by va_start.
 * @return Number of characters written, or a negative value on error.
 *
 * @note Windows CE 3.0 and later.
 */
int vprintf(const char *format, va_list arguments);
/**
 * Writes formatted output to a stream using a va_list.
 *
 * Initialize arguments with va_start. This function does not call va_end.
 *
 * @param stream Destination stream.
 * @param format Format-control string.
 * @param arguments Argument list initialized by va_start.
 * @return Number of characters written, or a negative value on error.
 *
 * @note Windows CE 3.0 and later.
 */
int vfprintf(FILE *stream, const char *format, va_list arguments);
/**
 * Formats output into buffer using a va_list and appends a null terminator.
 *
 * The buffer must be large enough for the output and terminator. Initialize
 * arguments with va_start; this function does not call va_end.
 *
 * @param buffer Receives the formatted string.
 * @param format Format-control string.
 * @param arguments Argument list initialized by va_start.
 * @return Number of characters written, excluding the null terminator, or
 *         a negative value on error.
 *
 * @note Windows CE 2.0 and later.
 */
int vsprintf(char *buffer, const char *format, va_list arguments);
/**
 * Formats output into a buffer using a va_list, storing no more than count
 * bytes.
 *
 * The terminating null is appended unless count is zero or the formatted
 * length is at least count. A truncated result returns a negative value.
 *
 * @param buffer Receives the formatted output.
 * @param count Maximum number of bytes to store.
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Bytes stored excluding the null terminator, or a negative value
 *         if the output does not fit.
 *
 * @note Windows CE 2.0 and later.
 */
int _vsnprintf(char *buffer, size_t count, const char *format, va_list arguments);
int snprintf(char *buffer, size_t count, const char *format, ...);
int remove(const char *path);
int rename(const char *existing, const char *replacement);
int vsnprintf(char *buffer, size_t count, const char *format, va_list arguments);
/**
 * Reads and converts formatted input from stdin.
 *
 * Each optional argument must point to a value of the type required by its
 * format specifier.
 *
 * @param format Format-control string.
 * @return Number of fields assigned, 0 if none were assigned, or EOF on
 *         error or end of input before the first conversion.
 *
 * @note Windows CE 3.0 and later.
 */
int scanf(const char *format, ...);
/**
 * Reads and converts formatted input from a stream.
 *
 * Each optional argument must point to a value of the type required by its
 * format specifier.
 *
 * @param stream Source stream.
 * @param format Format-control string.
 * @return Number of fields assigned, or EOF on error or end of file before
 *         the first conversion.
 *
 * @note Windows CE 3.0 and later.
 */
int fscanf(FILE *stream, const char *format, ...);
/**
 * Reads and converts formatted input from a string.
 *
 * Each optional argument must point to a value of the type required by its
 * format specifier.
 *
 * @param string Input string to scan.
 * @param format Format-control string.
 * @return Number of fields assigned, 0 if none were assigned, or EOF if
 *         input ends before the first conversion.
 *
 * @note Windows CE 2.0 and later.
 */
int sscanf(const char *string, const char *format, ...);

/**
 * Reads one wide character from a stream and advances its position.
 *
 * Use feof or ferror to distinguish end of file from a read error.
 *
 * @param stream Stream to read.
 * @return The character as wint_t, or WEOF on error or end of file.
 *
 * @note Windows CE 3.0 and later.
 */
wint_t fgetwc(FILE *stream);
/**
 * Writes one wide character to a stream.
 *
 * @param character Wide character to write.
 * @param stream Destination stream.
 * @return The character written, or WEOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
wint_t fputwc(wint_t character, FILE *stream);
/**
 * Pushes one wide character back onto a stream and clears its end-of-file
 * flag.
 *
 * The stream must be open for reading. A later read returns the pushed
 * character first.
 *
 * @param character Wide character to push back.
 * @param stream Stream to update.
 * @return The character on success, or WEOF if pushback fails.
 *
 * @note Windows CE 3.0 and later.
 */
wint_t ungetwc(wint_t character, FILE *stream);
/**
 * Reads at most size - 1 wide characters into buffer.
 *
 * Reading stops at a newline, end of file, or the size limit. A newline is
 * retained, and the result is null-terminated.
 *
 * @param buffer Receives the wide-character string.
 * @param size Buffer capacity in wide characters.
 * @param stream Stream to read.
 * @return buffer on success, or NULL on error or end of file.
 *
 * @note Windows CE 3.0 and later.
 */
wchar_t *fgetws(wchar_t *buffer, int size, FILE *stream);
/**
 * Writes a wide-character string to a stream without its null terminator.
 *
 * The stream's text or binary mode determines how wide characters are
 * written.
 *
 * @param string Null-terminated wide-character string.
 * @param stream Destination stream.
 * @return A nonnegative value on success, or WEOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
int fputws(const wchar_t *string, FILE *stream);
/**
 * Reads one wide character from stdin.
 *
 * @return The character as wint_t, or WEOF on error or end of file.
 *
 * @note Windows CE 3.0 and later.
 */
wint_t getwchar(void);
/**
 * Writes one wide character to stdout.
 *
 * @param character Wide character to write.
 * @return The character written, or WEOF on error.
 *
 * @note Windows CE 3.0 and later.
 */
wint_t putwchar(wint_t character);
/**
 * Reads a line from stdin into a wide-character buffer.
 *
 * The newline is replaced with a null terminator. The declaration has no
 * buffer-size argument, so the caller must provide enough storage.
 *
 * @param buffer Receives the input line.
 * @return buffer on success, or NULL on error or end of file.
 *
 * @note Windows CE 3.0 and later.
 */
wchar_t *_getws(wchar_t *buffer);
/**
 * Writes a wide-character string and a newline to stdout.
 *
 * @param string Null-terminated string to write.
 * @return A nonnegative value on success, or WEOF on failure.
 *
 * @note Windows CE 3.0 and later.
 */
int _putws(const wchar_t *string);
/**
 * Writes formatted wide-character output to stdout.
 *
 * The format string uses the printf format fields and wide-character
 * arguments.
 *
 * @param format Wide-character format-control string.
 * @return Number of wide characters written, or a negative value on error.
 *
 * @note Windows CE 3.0 and later.
 */
int wprintf(const wchar_t *format, ...);
/**
 * Writes formatted wide-character output to a stream.
 *
 * @param stream Destination stream.
 * @param format Wide-character format string.
 * @return Number of wide characters written, or a negative value on error.
 *
 * @note Windows CE 3.0 and later.
 */
int fwprintf(FILE *stream, const wchar_t *format, ...);
/**
 * Writes formatted wide-character output to stdout using a va_list.
 *
 * @param format Wide-character format-control string.
 * @param arguments Argument list.
 * @return Number of wide characters written, or a negative value on error.
 *
 * @note Windows CE 3.0 and later.
 */
int vwprintf(const wchar_t *format, va_list arguments);
/**
 * Writes formatted wide-character output to a stream using a va_list.
 *
 * Initialize arguments with va_start. This function does not call va_end.
 *
 * @param stream Destination stream.
 * @param format Wide-character format string.
 * @param arguments Argument list initialized by va_start.
 * @return Number of wide characters written, or a negative value on error.
 *
 * @note Windows CE 3.0 and later.
 */
int vfwprintf(FILE *stream, const wchar_t *format, va_list arguments);
/**
 * Reads and converts formatted wide-character input from stdin.
 *
 * Each optional argument must point to a value of the type required by its
 * format specifier.
 *
 * @param format Wide-character format-control string.
 * @return Number of fields assigned, 0 if none were assigned, or EOF on
 *         error or end of input before the first conversion.
 *
 * @note Windows CE 3.0 and later.
 */
int wscanf(const wchar_t *format, ...);
/**
 * Reads and converts formatted wide-character input from a stream.
 *
 * Each optional argument must point to a value of the type required by its
 * format specifier.
 *
 * @param stream Source stream.
 * @param format Wide-character format string.
 * @return Number of fields assigned, or EOF on error or end of file before
 *         the first conversion.
 *
 * @note Windows CE 3.0 and later.
 */
int fwscanf(FILE *stream, const wchar_t *format, ...);

void perror(const char *prefix);
void rewind(FILE *stream);
int grimace_redirect_stdio(const wchar_t *path);
/**
 * Sets the buffering of a stream.
 *
 * Call it after opening the stream and before any other operation on it.
 * Windows CE 4.1 and later.
 *
 * @param stream Open stream.
 * @param buffer Buffer to use, or NULL to have one allocated.
 * @param mode _IOFBF for full buffering, _IOLBF for line buffering or
 *        _IONBF for none.
 * @param size Size of the buffer in bytes.
 * @return 0 on success, nonzero for an invalid mode or size.
 */
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);
void setbuf(FILE *stream, char *buffer);

int getc(FILE *stream);
int putc(int character, FILE *stream);
wint_t getwc(FILE *stream);
wint_t putwc(wint_t character, FILE *stream);

#define getc fgetc
#define putc fputc
#define fileno _fileno
#define fcloseall _fcloseall
#define flushall _flushall

/**
 * Formats output into a buffer of known size, storing at most count
 * characters.
 *
 * The result is always null-terminated. If it doesn't fit and count is
 * _TRUNCATE, as much as fits is stored and -1 returned; otherwise buffer[0] is
 * set to 0 and the invalid parameter handler is called. Unlike the plain
 * version, the format string is checked: a NULL format or an invalid format
 * specifier calls the invalid parameter handler. Windows CE 6.0 and later.
 *
 * @param buffer Receives the formatted output.
 * @param size Size of buffer in characters, including the terminator.
 * @param count Maximum number of characters to store, or _TRUNCATE.
 * @param format Format-control string.
 * @return Characters written excluding the terminator, or -1 on truncation or
 *         error.
 */
int _snprintf_s(char *buffer, size_t size, size_t count, const char *format, ...);
/**
 * Formats output into a buffer of known size, storing at most count
 * characters.
 *
 * The result is always null-terminated. If it doesn't fit and count is
 * _TRUNCATE, as much as fits is stored and -1 returned; otherwise buffer[0] is
 * set to 0 and the invalid parameter handler is called. Unlike the plain
 * version, the format string is checked: a NULL format or an invalid format
 * specifier calls the invalid parameter handler. Windows CE 6.0 and later.
 *
 * @param buffer Receives the formatted output.
 * @param size Size of buffer in characters, including the terminator.
 * @param count Maximum number of characters to store, or _TRUNCATE.
 * @param format Format-control string.
 * @return Characters written excluding the terminator, or -1 on truncation or
 *         error.
 */
int _snwprintf_s(wchar_t *buffer, size_t size, size_t count, const wchar_t *format, ...);
/**
 * Formats output from a va_list into a buffer of known size, storing at most
 * count characters.
 *
 * The result is always null-terminated. If it doesn't fit and count is
 * _TRUNCATE, as much as fits is stored and -1 returned; otherwise buffer[0] is
 * set to 0 and the invalid parameter handler is called. Unlike the plain
 * version, the format string is checked: a NULL format or an invalid format
 * specifier calls the invalid parameter handler. Windows CE 6.0 and later.
 *
 * @param buffer Receives the formatted output.
 * @param size Size of buffer in characters, including the terminator.
 * @param count Maximum number of characters to store, or _TRUNCATE.
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Characters written excluding the terminator, or -1 on truncation or
 *         error.
 */
int _vsnprintf_s(char *buffer, size_t size, size_t count, const char *format, va_list arguments);
/**
 * Formats output from a va_list into a buffer of known size, storing at most
 * count characters.
 *
 * The result is always null-terminated. If it doesn't fit and count is
 * _TRUNCATE, as much as fits is stored and -1 returned; otherwise buffer[0] is
 * set to 0 and the invalid parameter handler is called. Unlike the plain
 * version, the format string is checked: a NULL format or an invalid format
 * specifier calls the invalid parameter handler. Windows CE 6.0 and later.
 *
 * @param buffer Receives the formatted output.
 * @param size Size of buffer in characters, including the terminator.
 * @param count Maximum number of characters to store, or _TRUNCATE.
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Characters written excluding the terminator, or -1 on truncation or
 *         error.
 */
int _vsnwprintf_s(wchar_t *buffer, size_t size, size_t count, const wchar_t *format, va_list arguments);
/**
 * Opens a file and returns the stream through a pointer.
 *
 * Takes the same mode strings as _wfopen. Files are opened without read or
 * write sharing. Bad arguments call the invalid parameter handler (see
 * _set_invalid_parameter_handler); if it returns, the function sets errno and
 * returns the error. Windows CE 6.0 and later.
 *
 * @param file Receives the new stream, or NULL on failure.
 * @param path File to open.
 * @param mode Access mode, as for _wfopen.
 * @return 0 on success, otherwise an errno value.
 */
errno_t _wfopen_s(FILE **file, const wchar_t *path, const wchar_t *mode);
/**
 * Closes a stream and reopens it on another file.
 *
 * Not listed in Microsoft's CE 6.0 table of security-enhanced functions, but
 * exported by coredll. Bad arguments call the invalid parameter handler (see
 * _set_invalid_parameter_handler); if it returns, the function sets errno and
 * returns the error. Windows CE 6.0 and later.
 *
 * @param file Receives the reopened stream, or NULL on failure.
 * @param path File to open.
 * @param mode Access mode, as for _wfopen.
 * @param stream Stream to close and reuse, such as stdout.
 * @return 0 on success, otherwise an errno value.
 */
errno_t _wfreopen_s(FILE **file, const wchar_t *path, const wchar_t *mode, FILE *stream);
/**
 * Resets the error and end-of-file indicators of a stream.
 *
 * Unlike clearerr it reports a NULL stream. Windows CE 6.0 and later.
 *
 * @param stream Stream to reset.
 * @return 0 on success, or EINVAL if stream is NULL.
 */
errno_t clearerr_s(FILE *stream);
/**
 * Opens a file and returns the stream through a pointer.
 *
 * Takes the same mode strings as fopen. Files are opened without read or write
 * sharing. Bad arguments call the invalid parameter handler (see
 * _set_invalid_parameter_handler); if it returns, the function sets errno and
 * returns the error. Windows CE 6.0 and later.
 *
 * @param file Receives the new stream, or NULL on failure.
 * @param path File to open.
 * @param mode Access mode, as for fopen.
 * @return 0 on success, otherwise an errno value.
 */
errno_t fopen_s(FILE **file, const char *path, const char *mode);
/**
 * Prints formatted output to a stream.
 *
 * Unlike the plain version, the format string is checked: a NULL format or an
 * invalid format specifier calls the invalid parameter handler. Windows CE 6.0
 * and later.
 *
 * @param stream Stream to write to.
 * @param format Format-control string.
 * @return Characters written, or a negative value on error.
 */
int fprintf_s(FILE *stream, const char *format, ...);
/**
 * Reads items from a stream into a buffer of known size.
 *
 * Fails rather than reading more than capacity bytes. Bad arguments call the
 * invalid parameter handler (see _set_invalid_parameter_handler); if it
 * returns, the function sets errno and returns the error. Windows CE 6.0 and
 * later.
 *
 * @param buffer Receives the data.
 * @param capacity Size of buffer in bytes.
 * @param size Size of one item in bytes.
 * @param count Maximum number of items to read.
 * @param stream Stream to read from.
 * @return Number of complete items read, which may be less than count at end
 *         of file or on error.
 */
size_t fread_s(void *buffer, size_t capacity, size_t size, size_t count, FILE *stream);
/**
 * Reads formatted data from a stream.
 *
 * The %c, %s and %[ conversions take a buffer size, as an unsigned int
 * argument straight after the buffer pointer, and stop rather than overrun it.
 * Windows CE 6.0 and later.
 *
 * @param stream Stream to read from.
 * @param format Format-control string.
 * @return Number of fields converted and assigned, or EOF at end of input or
 *         on error.
 */
int fscanf_s(FILE *stream, const char *format, ...);
/**
 * Prints formatted output to a stream.
 *
 * Unlike the plain version, the format string is checked: a NULL format or an
 * invalid format specifier calls the invalid parameter handler. Windows CE 6.0
 * and later.
 *
 * @param stream Stream to write to.
 * @param format Format-control string.
 * @return Characters written, or a negative value on error.
 */
int fwprintf_s(FILE *stream, const wchar_t *format, ...);
/**
 * Reads formatted data from a stream.
 *
 * The %c, %s and %[ conversions take a buffer size, as an unsigned int
 * argument straight after the buffer pointer, and stop rather than overrun it.
 * Windows CE 6.0 and later.
 *
 * @param stream Stream to read from.
 * @param format Format-control string.
 * @return Number of fields converted and assigned, or EOF at end of input or
 *         on error.
 */
int fwscanf_s(FILE *stream, const wchar_t *format, ...);
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
char *gets_s(char *buffer, size_t size);
/**
 * Prints formatted output to stdout.
 *
 * Unlike the plain version, the format string is checked: a NULL format or an
 * invalid format specifier calls the invalid parameter handler. Windows CE 6.0
 * and later.
 *
 * @param format Format-control string.
 * @return Characters written, or a negative value on error.
 */
int printf_s(const char *format, ...);
/**
 * Reads formatted data from stdin.
 *
 * The %c, %s and %[ conversions take a buffer size, as an unsigned int
 * argument straight after the buffer pointer, and stop rather than overrun it.
 * Windows CE 6.0 and later.
 *
 * @param format Format-control string.
 * @return Number of fields converted and assigned, or EOF at end of input or
 *         on error.
 */
int scanf_s(const char *format, ...);
/**
 * Formats output into a buffer of known size.
 *
 * The result is always null-terminated. If it doesn't fit, buffer[0] is set to
 * 0 and the invalid parameter handler is called. Unlike the plain version, the
 * format string is checked: a NULL format or an invalid format specifier calls
 * the invalid parameter handler. Windows CE 6.0 and later.
 *
 * @param buffer Receives the formatted output.
 * @param size Size of buffer in characters, including the terminator.
 * @param format Format-control string.
 * @return Characters written excluding the terminator, or -1 on error.
 */
int sprintf_s(char *buffer, size_t size, const char *format, ...);
/**
 * Reads formatted data from a string.
 *
 * The %c, %s and %[ conversions take a buffer size, as an unsigned int
 * argument straight after the buffer pointer, and stop rather than overrun it.
 * Windows CE 6.0 and later.
 *
 * @param string String to read from.
 * @param format Format-control string.
 * @return Number of fields converted and assigned, or EOF if the string ends
 *         first or on error.
 */
int sscanf_s(const char *string, const char *format, ...);
/**
 * Formats output into a buffer of known size.
 *
 * The result is always null-terminated. If it doesn't fit, buffer[0] is set to
 * 0 and the invalid parameter handler is called. Unlike the plain version, the
 * format string is checked: a NULL format or an invalid format specifier calls
 * the invalid parameter handler. Windows CE 6.0 and later.
 *
 * @param buffer Receives the formatted output.
 * @param size Size of buffer in characters, including the terminator.
 * @param format Format-control string.
 * @return Characters written excluding the terminator, or -1 on error.
 */
int swprintf_s(wchar_t *buffer, size_t size, const wchar_t *format, ...);
/**
 * Reads formatted data from a string.
 *
 * The %c, %s and %[ conversions take a buffer size, as an unsigned int
 * argument straight after the buffer pointer, and stop rather than overrun it.
 * Windows CE 6.0 and later.
 *
 * @param string String to read from.
 * @param format Format-control string.
 * @return Number of fields converted and assigned, or EOF if the string ends
 *         first or on error.
 */
int swscanf_s(const wchar_t *string, const wchar_t *format, ...);
/**
 * Prints formatted output from a va_list to a stream.
 *
 * Unlike the plain version, the format string is checked: a NULL format or an
 * invalid format specifier calls the invalid parameter handler. Windows CE 6.0
 * and later.
 *
 * @param stream Stream to write to.
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Characters written, or a negative value on error.
 */
int vfprintf_s(FILE *stream, const char *format, va_list arguments);
/**
 * Prints formatted output from a va_list to a stream.
 *
 * Unlike the plain version, the format string is checked: a NULL format or an
 * invalid format specifier calls the invalid parameter handler. Windows CE 6.0
 * and later.
 *
 * @param stream Stream to write to.
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Characters written, or a negative value on error.
 */
int vfwprintf_s(FILE *stream, const wchar_t *format, va_list arguments);
/**
 * Prints formatted output from a va_list to stdout.
 *
 * Unlike the plain version, the format string is checked: a NULL format or an
 * invalid format specifier calls the invalid parameter handler. Windows CE 6.0
 * and later.
 *
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Characters written, or a negative value on error.
 */
int vprintf_s(const char *format, va_list arguments);
/**
 * Formats output from a va_list into a buffer of known size.
 *
 * The result is always null-terminated. If it doesn't fit, buffer[0] is set to
 * 0 and the invalid parameter handler is called. Unlike the plain version, the
 * format string is checked: a NULL format or an invalid format specifier calls
 * the invalid parameter handler. Windows CE 6.0 and later.
 *
 * @param buffer Receives the formatted output.
 * @param size Size of buffer in characters, including the terminator.
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Characters written excluding the terminator, or -1 on error.
 */
int vsprintf_s(char *buffer, size_t size, const char *format, va_list arguments);
/**
 * Formats output from a va_list into a buffer of known size.
 *
 * The result is always null-terminated. If it doesn't fit, buffer[0] is set to
 * 0 and the invalid parameter handler is called. Unlike the plain version, the
 * format string is checked: a NULL format or an invalid format specifier calls
 * the invalid parameter handler. Windows CE 6.0 and later.
 *
 * @param buffer Receives the formatted output.
 * @param size Size of buffer in characters, including the terminator.
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Characters written excluding the terminator, or -1 on error.
 */
int vswprintf_s(wchar_t *buffer, size_t size, const wchar_t *format, va_list arguments);
/**
 * Prints formatted output from a va_list to stdout.
 *
 * Unlike the plain version, the format string is checked: a NULL format or an
 * invalid format specifier calls the invalid parameter handler. Windows CE 6.0
 * and later.
 *
 * @param format Format-control string.
 * @param arguments Argument list.
 * @return Characters written, or a negative value on error.
 */
int vwprintf_s(const wchar_t *format, va_list arguments);
/**
 * Prints formatted output to stdout.
 *
 * Unlike the plain version, the format string is checked: a NULL format or an
 * invalid format specifier calls the invalid parameter handler. Windows CE 6.0
 * and later.
 *
 * @param format Format-control string.
 * @return Characters written, or a negative value on error.
 */
int wprintf_s(const wchar_t *format, ...);
/**
 * Reads formatted data from stdin.
 *
 * The %c, %s and %[ conversions take a buffer size, as an unsigned int
 * argument straight after the buffer pointer, and stop rather than overrun it.
 * Windows CE 6.0 and later.
 *
 * @param format Format-control string.
 * @return Number of fields converted and assigned, or EOF at end of input or
 *         on error.
 */
int wscanf_s(const wchar_t *format, ...);

#ifdef __cplusplus
}
#endif

#ifdef GRIMACE_IOSTREAMS
#ifdef __cplusplus
extern "C" {
#endif
int fseeko(FILE *stream, long offset, int origin) __asm__("fseek");
long ftello(FILE *stream) __asm__("ftell");
FILE *fdopen(int descriptor, const char *mode);
int _fseeki64(FILE *stream, long long offset, int origin);
long long _ftelli64(FILE *stream);
#ifdef __cplusplus
}
#endif
#endif

#ifdef GRIMACE_POSIX
#include <grimace/posix-stdio.h>
#endif

#endif
