#ifndef GRIMACE_STRSAFE_H
#define GRIMACE_STRSAFE_H

#include <windows.h>
#include <stdarg.h>

#define STRSAFE_MAX_CCH 2147483647
#define STRSAFE_IGNORE_NULLS 0x00000100
#define STRSAFE_FILL_BEHIND_NULL 0x00000200
#define STRSAFE_FILL_ON_FAILURE 0x00000400
#define STRSAFE_NULL_ON_FAILURE 0x00000800
#define STRSAFE_NO_TRUNCATION 0x00001000
#define STRSAFE_E_INSUFFICIENT_BUFFER ((HRESULT)0x8007007AL)
#define STRSAFE_E_INVALID_PARAMETER ((HRESULT)0x80070057L)
#define STRSAFE_E_END_OF_FILE ((HRESULT)0x80070026L)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Appends one ANSI string to another, never writing past the end of the
 * destination buffer.
 *
 * Replaces strcat. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszSrc Null-terminated string to append.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCatA(LPSTR pszDest, size_t cbDest, LPCSTR pszSrc);
/**
 * Appends one ANSI string to another, never writing past the end of the
 * destination buffer.
 *
 * Like StringCbCatA, but also reports where the result ends and how much space
 * is left, and takes flags that control NULL handling and what happens on
 * failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszSrc Null-terminated string to append.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCatExA(LPSTR pszDest, size_t cbDest, LPCSTR pszSrc, LPSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags);
/**
 * Appends one wide string to another, never writing past the end of the
 * destination buffer.
 *
 * Like StringCbCatW, but also reports where the result ends and how much space
 * is left, and takes flags that control NULL handling and what happens on
 * failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszSrc Null-terminated string to append.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCatExW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszSrc, LPWSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags);
/**
 * Appends at most cbMaxAppend bytes of an ANSI string to another, never
 * writing past the end of the destination buffer.
 *
 * Replaces strncat. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszSrc Null-terminated string to append.
 * @param cbMaxAppend Maximum number of bytes of pszSrc to append.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCatNA(LPSTR pszDest, size_t cbDest, LPCSTR pszSrc, size_t cbMaxAppend);
/**
 * Appends at most cbMaxAppend bytes of an ANSI string to another, never
 * writing past the end of the destination buffer.
 *
 * Like StringCbCatNA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszSrc Null-terminated string to append.
 * @param cbMaxAppend Maximum number of bytes of pszSrc to append.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCatNExA(LPSTR pszDest, size_t cbDest, LPCSTR pszSrc, size_t cbMaxAppend, LPSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags);
/**
 * Appends at most cbMaxAppend bytes of a wide string to another, never writing
 * past the end of the destination buffer.
 *
 * Like StringCbCatNW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszSrc Null-terminated string to append.
 * @param cbMaxAppend Maximum number of bytes of pszSrc to append.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCatNExW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszSrc, size_t cbMaxAppend, LPWSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags);
/**
 * Appends at most cbMaxAppend bytes of a wide string to another, never writing
 * past the end of the destination buffer.
 *
 * Replaces wcsncat. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszSrc Null-terminated string to append.
 * @param cbMaxAppend Maximum number of bytes of pszSrc to append.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCatNW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszSrc, size_t cbMaxAppend);
/**
 * Appends one wide string to another, never writing past the end of the
 * destination buffer.
 *
 * Replaces wcscat. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszSrc Null-terminated string to append.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCatW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszSrc);
/**
 * Copies an ANSI string into a buffer of known size, truncating if it doesn't
 * fit.
 *
 * Replaces strcpy. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszSrc Null-terminated string to copy.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCopyA(LPSTR pszDest, size_t cbDest, LPCSTR pszSrc);
/**
 * Copies an ANSI string into a buffer of known size, truncating if it doesn't
 * fit.
 *
 * Like StringCbCopyA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszSrc Null-terminated string to copy.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCopyExA(LPSTR pszDest, size_t cbDest, LPCSTR pszSrc, LPSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags);
/**
 * Copies a wide string into a buffer of known size, truncating if it doesn't
 * fit.
 *
 * Like StringCbCopyW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszSrc Null-terminated string to copy.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCopyExW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszSrc, LPWSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags);
/**
 * Copies at most cbSrc bytes of an ANSI string into a buffer of known size.
 *
 * Replaces strncpy. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Unlike strncpy, it does not pad the rest of the buffer with nulls.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszSrc Null-terminated string to copy.
 * @param cbSrc Maximum number of bytes to copy from pszSrc.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCopyNA(LPSTR pszDest, size_t cbDest, LPCSTR pszSrc, size_t cbSrc);
/**
 * Copies at most cbSrc bytes of a wide string into a buffer of known size.
 *
 * Like StringCbCopyNW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Unlike wcsncpy, it does not pad the rest of the buffer with nulls.
 *
 * Overlapping strings give undefined results. Windows CE 5.0 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszSrc Null-terminated string to copy.
 * @param cbSrc Maximum number of bytes to copy from pszSrc.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCopyNExW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszSrc, size_t cbSrc, LPWSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags);
/**
 * Copies at most cbSrc bytes of a wide string into a buffer of known size.
 *
 * Replaces wcsncpy. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Unlike wcsncpy, it does not pad the rest of the buffer with nulls.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszSrc Null-terminated string to copy.
 * @param cbSrc Maximum number of bytes to copy from pszSrc.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCopyNW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszSrc, size_t cbSrc);
/**
 * Copies a wide string into a buffer of known size, truncating if it doesn't
 * fit.
 *
 * Replaces wcscpy. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszSrc Null-terminated string to copy.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbCopyW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszSrc);
/**
 * Gets the length of an ANSI string, failing if it is longer than a given
 * maximum.
 *
 * Replaces strlen. Windows CE 4.1 and later.
 *
 * @param psz String to measure.
 * @param cbMax Maximum length allowed in bytes, including the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pcb Receives the length in bytes, excluding the terminating null. Can
 *        be NULL to just check the string fits.
 * @return S_OK if the string fits within cbMax, or STRSAFE_E_INVALID_PARAMETER
 *         if psz is NULL, cbMax is too large, or the string is longer than
 *         cbMax.
 */
HRESULT WINAPI StringCbLengthA(LPCSTR psz, size_t cbMax, size_t *pcb);
/**
 * Gets the length of a wide string, failing if it is longer than a given
 * maximum.
 *
 * Replaces wcslen. Windows CE 4.1 and later.
 *
 * @param psz String to measure.
 * @param cbMax Maximum length allowed in bytes, including the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pcb Receives the length in bytes, excluding the terminating null. Can
 *        be NULL to just check the string fits.
 * @return S_OK if the string fits within cbMax, or STRSAFE_E_INVALID_PARAMETER
 *         if psz is NULL, cbMax is too large, or the string is longer than
 *         cbMax.
 */
HRESULT WINAPI StringCbLengthW(LPCWSTR psz, size_t cbMax, size_t *pcb);
/**
 * Formats arguments into a buffer of known size, in the manner of sprintf.
 *
 * Replaces sprintf. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszFormat printf-style format string.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbPrintfA(LPSTR pszDest, size_t cbDest, LPCSTR pszFormat, ...);
/**
 * Formats arguments into a buffer of known size, in the manner of sprintf.
 *
 * Like StringCbPrintfA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @param pszFormat printf-style format string.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbPrintfExA(LPSTR pszDest, size_t cbDest, LPSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags, LPCSTR pszFormat, ...);
/**
 * Formats arguments into a buffer of known size, in the manner of swprintf.
 *
 * Like StringCbPrintfW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @param pszFormat printf-style format string.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbPrintfExW(LPWSTR pszDest, size_t cbDest, LPWSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags, LPCWSTR pszFormat, ...);
/**
 * Formats arguments into a buffer of known size, in the manner of swprintf.
 *
 * Replaces swprintf. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszFormat printf-style format string.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbPrintfW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszFormat, ...);
/**
 * Formats a va_list into a buffer of known size, in the manner of vsprintf.
 *
 * Replaces vsprintf. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param pszFormat printf-style format string.
 * @param argList Arguments for pszFormat.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbVPrintfA(LPSTR pszDest, size_t cbDest, LPCSTR pszFormat, va_list argList);
/**
 * Formats a va_list into a buffer of known size, in the manner of vsprintf.
 *
 * Like StringCbVPrintfA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(char).
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @param pszFormat printf-style format string.
 * @param argList Arguments for pszFormat.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbVPrintfExA(LPSTR pszDest, size_t cbDest, LPSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags, LPCSTR pszFormat, va_list argList);
/**
 * Formats a va_list into a buffer of known size, in the manner of vswprintf.
 *
 * Like StringCbVPrintfW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcbRemaining Receives the number of unused bytes left in pszDest,
 *        counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @param pszFormat printf-style format string.
 * @param argList Arguments for pszFormat.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbVPrintfExW(LPWSTR pszDest, size_t cbDest, LPWSTR *ppszDestEnd, size_t *pcbRemaining, DWORD dwFlags, LPCWSTR pszFormat, va_list argList);
/**
 * Formats a va_list into a buffer of known size, in the manner of vswprintf.
 *
 * Replaces vswprintf. The result is always null-terminated when cbDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cbDest Size of pszDest in bytes, including room for the terminating
 *        null. At most STRSAFE_MAX_CCH * sizeof(WCHAR).
 * @param pszFormat printf-style format string.
 * @param argList Arguments for pszFormat.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cbDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCbVPrintfW(LPWSTR pszDest, size_t cbDest, LPCWSTR pszFormat, va_list argList);
/**
 * Appends one ANSI string to another, never writing past the end of the
 * destination buffer.
 *
 * Replaces strcat. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to append.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCatA(LPSTR pszDest, size_t cchDest, LPCSTR pszSrc);
/**
 * Appends one ANSI string to another, never writing past the end of the
 * destination buffer.
 *
 * Like StringCchCatA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to append.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCatExA(LPSTR pszDest, size_t cchDest, LPCSTR pszSrc, LPSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags);
/**
 * Appends one wide string to another, never writing past the end of the
 * destination buffer.
 *
 * Like StringCchCatW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to append.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCatExW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc, LPWSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags);
/**
 * Appends at most cchMaxAppend characters of an ANSI string to another, never
 * writing past the end of the destination buffer.
 *
 * Replaces strncat. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to append.
 * @param cchMaxAppend Maximum number of characters of pszSrc to append.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCatNA(LPSTR pszDest, size_t cchDest, LPCSTR pszSrc, size_t cchMaxAppend);
/**
 * Appends at most cchMaxAppend characters of an ANSI string to another, never
 * writing past the end of the destination buffer.
 *
 * Like StringCchCatNA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to append.
 * @param cchMaxAppend Maximum number of characters of pszSrc to append.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCatNExA(LPSTR pszDest, size_t cchDest, LPCSTR pszSrc, size_t cchMaxAppend, LPSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags);
/**
 * Appends at most cchMaxAppend characters of a wide string to another, never
 * writing past the end of the destination buffer.
 *
 * Like StringCchCatNW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to append.
 * @param cchMaxAppend Maximum number of characters of pszSrc to append.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCatNExW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc, size_t cchMaxAppend, LPWSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags);
/**
 * Appends at most cchMaxAppend characters of a wide string to another, never
 * writing past the end of the destination buffer.
 *
 * Replaces wcsncat. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to append.
 * @param cchMaxAppend Maximum number of characters of pszSrc to append.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCatNW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc, size_t cchMaxAppend);
/**
 * Appends one wide string to another, never writing past the end of the
 * destination buffer.
 *
 * Replaces wcscat. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Buffer holding the string to append to, which receives the
 *        result.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to append.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCatW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc);
/**
 * Copies an ANSI string into a buffer of known size, truncating if it doesn't
 * fit.
 *
 * Replaces strcpy. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to copy.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCopyA(LPSTR pszDest, size_t cchDest, LPCSTR pszSrc);
/**
 * Copies an ANSI string into a buffer of known size, truncating if it doesn't
 * fit.
 *
 * Like StringCchCopyA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to copy.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCopyExA(LPSTR pszDest, size_t cchDest, LPCSTR pszSrc, LPSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags);
/**
 * Copies a wide string into a buffer of known size, truncating if it doesn't
 * fit.
 *
 * Like StringCchCopyW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to copy.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCopyExW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc, LPWSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags);
/**
 * Copies at most cchSrc characters of an ANSI string into a buffer of known
 * size.
 *
 * Replaces strncpy. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Unlike strncpy, it does not pad the rest of the buffer with nulls.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to copy.
 * @param cchSrc Maximum number of characters to copy from pszSrc.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCopyNA(LPSTR pszDest, size_t cchDest, LPCSTR pszSrc, size_t cchSrc);
/**
 * Copies at most cchSrc characters of a wide string into a buffer of known
 * size.
 *
 * Like StringCchCopyNW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings.
 *
 * Unlike wcsncpy, it does not pad the rest of the buffer with nulls.
 *
 * Overlapping strings give undefined results. Windows CE 5.0 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to copy.
 * @param cchSrc Maximum number of characters to copy from pszSrc.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCopyNExW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc, size_t cchSrc, LPWSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags);
/**
 * Copies at most cchSrc characters of a wide string into a buffer of known
 * size.
 *
 * Replaces wcsncpy. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Unlike wcsncpy, it does not pad the rest of the buffer with nulls.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to copy.
 * @param cchSrc Maximum number of characters to copy from pszSrc.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCopyNW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc, size_t cchSrc);
/**
 * Copies a wide string into a buffer of known size, truncating if it doesn't
 * fit.
 *
 * Replaces wcscpy. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Neither string may be NULL; use the Ex form
 * with STRSAFE_IGNORE_NULLS for that.
 *
 * Overlapping strings give undefined results. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the copied string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszSrc Null-terminated string to copy.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchCopyW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc);
/**
 * Gets the length of an ANSI string, failing if it is longer than a given
 * maximum.
 *
 * Replaces strlen. Windows CE 4.1 and later.
 *
 * @param psz String to measure.
 * @param cchMax Maximum length allowed in characters, including the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pcch Receives the length in characters, excluding the terminating
 *        null. Can be NULL to just check the string fits.
 * @return S_OK if the string fits within cchMax, or
 *         STRSAFE_E_INVALID_PARAMETER if psz is NULL, cchMax is too large, or
 *         the string is longer than cchMax.
 */
HRESULT WINAPI StringCchLengthA(LPCSTR psz, size_t cchMax, size_t *pcch);
/**
 * Gets the length of a wide string, failing if it is longer than a given
 * maximum.
 *
 * Replaces wcslen. Windows CE 4.1 and later.
 *
 * @param psz String to measure.
 * @param cchMax Maximum length allowed in characters, including the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pcch Receives the length in characters, excluding the terminating
 *        null. Can be NULL to just check the string fits.
 * @return S_OK if the string fits within cchMax, or
 *         STRSAFE_E_INVALID_PARAMETER if psz is NULL, cchMax is too large, or
 *         the string is longer than cchMax.
 */
HRESULT WINAPI StringCchLengthW(LPCWSTR psz, size_t cchMax, size_t *pcch);
/**
 * Formats arguments into a buffer of known size, in the manner of sprintf.
 *
 * Replaces sprintf. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszFormat printf-style format string.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchPrintfA(LPSTR pszDest, size_t cchDest, LPCSTR pszFormat, ...);
/**
 * Formats arguments into a buffer of known size, in the manner of sprintf.
 *
 * Like StringCchPrintfA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @param pszFormat printf-style format string.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchPrintfExA(LPSTR pszDest, size_t cchDest, LPSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags, LPCSTR pszFormat, ...);
/**
 * Formats arguments into a buffer of known size, in the manner of swprintf.
 *
 * Like StringCchPrintfW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @param pszFormat printf-style format string.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchPrintfExW(LPWSTR pszDest, size_t cchDest, LPWSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags, LPCWSTR pszFormat, ...);
/**
 * Formats arguments into a buffer of known size, in the manner of swprintf.
 *
 * Replaces swprintf. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszFormat printf-style format string.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchPrintfW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszFormat, ...);
/**
 * Formats a va_list into a buffer of known size, in the manner of vsprintf.
 *
 * Replaces vsprintf. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszFormat printf-style format string.
 * @param argList Arguments for pszFormat.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchVPrintfA(LPSTR pszDest, size_t cchDest, LPCSTR pszFormat, va_list argList);
/**
 * Formats a va_list into a buffer of known size, in the manner of vsprintf.
 *
 * Like StringCchVPrintfA, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @param pszFormat printf-style format string.
 * @param argList Arguments for pszFormat.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchVPrintfExA(LPSTR pszDest, size_t cchDest, LPSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags, LPCSTR pszFormat, va_list argList);
/**
 * Formats a va_list into a buffer of known size, in the manner of vswprintf.
 *
 * Like StringCchVPrintfW, but also reports where the result ends and how much
 * space is left, and takes flags that control NULL handling and what happens
 * on failure. What is left in pszDest after a failure depends on dwFlags; by
 * default it holds the truncated, null-terminated result. STRSAFE_IGNORE_NULLS
 * treats NULL string pointers as empty strings. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param ppszDestEnd Receives a pointer to the terminating null of the result.
 *        Can be NULL.
 * @param pcchRemaining Receives the number of unused characters left in
 *        pszDest, counting the terminating null. Can be NULL.
 * @param dwFlags Zero or more of STRSAFE_IGNORE_NULLS,
 *        STRSAFE_FILL_BEHIND_NULL, STRSAFE_FILL_ON_FAILURE,
 *        STRSAFE_NULL_ON_FAILURE and STRSAFE_NO_TRUNCATION. The low byte is
 *        the fill value used by the FILL flags.
 * @param pszFormat printf-style format string.
 * @param argList Arguments for pszFormat.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0, too large, or
 *         pszDest is already full. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchVPrintfExW(LPWSTR pszDest, size_t cchDest, LPWSTR *ppszDestEnd, size_t *pcchRemaining, DWORD dwFlags, LPCWSTR pszFormat, va_list argList);
/**
 * Formats a va_list into a buffer of known size, in the manner of vswprintf.
 *
 * Replaces vswprintf. The result is always null-terminated when cchDest is
 * non-zero. A result that doesn't fit is truncated and still null-terminated,
 * and the function returns STRSAFE_E_INSUFFICIENT_BUFFER. Where truncation is
 * acceptable that is not an error. Windows CE 4.1 and later.
 *
 * @param pszDest Receives the formatted string.
 * @param cchDest Size of pszDest in characters, including room for the
 *        terminating null. At most STRSAFE_MAX_CCH.
 * @param pszFormat printf-style format string.
 * @param argList Arguments for pszFormat.
 * @return S_OK on success, STRSAFE_E_INSUFFICIENT_BUFFER if the result did not
 *         fit, or STRSAFE_E_INVALID_PARAMETER if cchDest is 0 or too large.
 *         Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI StringCchVPrintfW(LPWSTR pszDest, size_t cchDest, LPCWSTR pszFormat, va_list argList);

#ifdef __cplusplus
}
#endif

#if !defined(RC_INVOKED) && !defined(GRIMACE_ALL_DECLARATIONS)
#include <grimace/aliases.h>
#include <grimace/unavailable.h>
#include <grimace/lazy.h>
#endif

#endif
