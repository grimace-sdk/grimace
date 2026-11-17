#ifndef _WINDEF_H
#define _WINDEF_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINVER
#define WINVER 0x0400
/*
 * If you need Win32 API features newer the Win95 and WinNT then you must
 * define WINVER before including windows.h or any other method of including
 * the windef.h header.
 */
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT WINVER
/*
 * There may be the need to define _WIN32_WINNT to a value different from
 * the value of WINVER.  I don't have any example of why you would do that.
 * However, if you must then define _WIN32_WINNT to the value required before
 * including windows.h or any other method of including the windef.h header.
 */
#endif

#if defined(__CEGCC__) || defined(__MINGW32CE__)
#ifndef _WIN32_WCE
#define _WIN32_WCE 0x0300
/*
 * If you need WinCE API features newer than WinCE 3.0 then you must
 * define _WIN32_WCE to the value required before including windows.h 
 * or any other method of including the windef.h header.
 */
#endif
#endif

/*
 * Macro to deal with wide names - WinCE functions not receiving LPTSTR
 * directly don't have W suffix, even though they are implicitly wide
 * (expect wide strings in structures passed, etc.)
 */
#ifdef _WIN32_WCE
#define _WNAME(name) name
#else
#define _WNAME(name) name ## W
#endif

#ifndef WIN32
#define WIN32
#endif
#ifndef _WIN32
#define _WIN32
#endif
#define FAR
#define far
#define NEAR
#define near
#ifndef CONST
#define CONST const
#endif
#undef MAX_PATH
#define MAX_PATH 260

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/* Pseudo modifiers for parameters
   We don't use these unnecessary defines in the w32api headers. Define
   them by default since that is what people expect, but allow users
   to avoid the pollution.  */
#ifndef _NO_W32_PSEUDO_MODIFIERS
#define IN
#define OUT
#ifndef OPTIONAL
#define OPTIONAL
#endif
#endif

#ifdef __GNUC__
#define PACKED __attribute__((packed))
#ifndef _fastcall
#define _fastcall __attribute__((fastcall))
#endif
#ifndef __fastcall
#define __fastcall __attribute__((fastcall))
#endif
#ifndef _stdcall
#define _stdcall __attribute__((stdcall))
#endif
#ifndef __stdcall
#define __stdcall __attribute__((stdcall))
#endif
#ifndef _cdecl
#define _cdecl __attribute__((cdecl))
#endif
#ifndef __cdecl
#define __cdecl __attribute__((cdecl))
#endif
#ifndef __declspec
#define __declspec(e) __attribute__((e))
#endif
#ifndef _declspec
#define _declspec(e) __attribute__((e))
#endif
#elif defined(__WATCOMC__)
#define PACKED
#else
#define PACKED
#define _cdecl
#define __cdecl
#endif

#undef pascal
#undef _pascal
#undef __pascal
#define pascal __stdcall
#define _pascal __stdcall
#define __pascal __stdcall
#define PASCAL _pascal
#define CDECL _cdecl
#define STDCALL __stdcall
#define FASTCALL __fastcall
/**
 * Calling convention of every Windows API function, placed between the
 * return type and the name: `int WINAPI WinMain(...)`.
 *
 * It expands to nothing on Windows CE. The system DLLs use the compiler's
 * one calling convention on each processor, so there is no __stdcall to
 * declare, and the headers define __stdcall and __cdecl as empty. Keep it
 * in declarations for source compatibility with desktop Windows.
 */
#define WINAPI __stdcall
/**
 * Calling convention of the variadic API functions (wsprintfW): __cdecl on
 * desktop Windows, nothing on Windows CE, where every function uses the
 * compiler's one convention.
 */
#define WINAPIV __cdecl
/**
 * Another name for WINAPI, used by the older declarations and the OpenGL
 * ones. Empty on Windows CE.
 */
#define APIENTRY __stdcall
/**
 * Calling convention of functions the system calls back: window and dialog
 * procedures, enumeration and timer callbacks. The same as WINAPI, and like
 * it empty on Windows CE.
 */
#define CALLBACK __stdcall
#define APIPRIVATE __stdcall

#define DECLSPEC_IMPORT __declspec(dllimport)
#define DECLSPEC_EXPORT __declspec(dllexport)
#ifdef __GNUC__
#define DECLSPEC_NORETURN __declspec(noreturn)
#define DECLARE_STDCALL_P( type ) __stdcall type
#elif defined(__WATCOMC__)
#define DECLSPEC_NORETURN
#define DECLARE_STDCALL_P( type ) type __stdcall
#endif /* __GNUC__/__WATCOMC__ */
/**
 * This macro creates an unsigned 16-bit integer by concatenating two
 * spscified unsigned character values.
 *
 * The MAKEWORD macro is defined as follows:
 *
 *     #define MAKEWORD(a, b) \
 *         ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8))
 *
 * @param bLow Specifies the low-order byte of the new short value.
 * @param bHigh Specifies the high-order byte of the new short value.
 * @return The return value is an unsigned 16-bit integer value.
 */
#define MAKEWORD(a,b)	((WORD)(((BYTE)(a))|(((WORD)((BYTE)(b)))<<8)))
/**
 * This macro creates an unsigned 32-bit value by concatenating two
 * specified 16-bit values.
 *
 * The MAKELONG macro is defined as follows:
 *
 *     #define MAKELONG(a, b) \
 *         ((LONG) (((WORD) (a)) | ((DWORD) ((WORD) (b))) << 16))
 *
 * @param wLow Specifies the low-order word of the new long value.
 * @param wHigh Specifies the high-order word of the new long value.
 * @return The return value is an unsigned 32-bit value.
 */
#define MAKELONG(a,b)	((LONG)(((WORD)(a))|(((DWORD)((WORD)(b)))<<16)))
/**
 * This macro retrieves the low-order word from the specified 32-bit value.
 *
 * The LOWORD macro is defined as follows:
 *
 *     #define LOWORD(l)   ((WORD) (l))
 *
 * @param dwValue Specifies the value to be converted.
 * @return The return value is the low-order word of the specified value.
 */
#define LOWORD(l)	((WORD)((DWORD)(l)))
/**
 * This macro retrieves the high-order word from the specified 32-bit
 * value.
 *
 * The HIWORD macro is defined as follows:
 *
 *     #define HIWORD(l)   ((WORD) (((DWORD) (l) >> 16) & 0xFFFF))
 *
 * @param dwValue Specifies the value to be converted.
 * @return The return value is the high-order word of the specified value.
 */
#define HIWORD(l)	((WORD)(((DWORD)(l)>>16)&0xFFFF))
/**
 * This macro retrieves the low-order byte from the specified 16-bit value.
 *
 * The LOBYTE macro is defined as follows:
 *
 *     #define LOBYTE(w)   ((BYTE) (w))
 *
 * @param wValue Specifies the value to be converted.
 * @return The return value is the low-order byte of the specified value.
 */
#define LOBYTE(w)	((BYTE)(w))
/**
 * This macro retrieves the high-order byte from the specified 16-bit
 * value.
 *
 * The HIBYTE macro is defined as follows:
 *
 *     #define HIBYTE(w)   ((BYTE) (((WORD) (w) >> 8) & 0xFF))
 *
 * @param wValue Specifies the value to be converted.
 * @return The return value is the high-order byte of the specified value.
 */
#define HIBYTE(w)	((BYTE)(((WORD)(w)>>8)&0xFF))

#ifndef __WATCOMC__
#ifndef _export
#define _export
#endif
#ifndef __export
#define __export
#endif
#endif

#ifndef NOMINMAX
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#endif

#define UNREFERENCED_PARAMETER(P) {(P)=(P);}
#define UNREFERENCED_LOCAL_VARIABLE(L) {(L)=(L);}
#define DBG_UNREFERENCED_PARAMETER(P)
#define DBG_UNREFERENCED_LOCAL_VARIABLE(L)

#ifndef NONAMELESSUNION
#ifdef __GNUC__
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#define _ANONYMOUS_UNION __extension__
#define _ANONYMOUS_STRUCT __extension__
#else
#if defined(__cplusplus)
#define _ANONYMOUS_UNION __extension__
#endif /* __cplusplus */
#endif /* __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95) */
#elif defined(__WATCOMC__)
#define _ANONYMOUS_UNION
#define _ANONYMOUS_STRUCT
#endif /* __GNUC__/__WATCOMC__ */
#endif /* NONAMELESSUNION */

#ifndef _ANONYMOUS_UNION
#define _ANONYMOUS_UNION
#define _UNION_NAME(x) x
#define DUMMYUNIONNAME	u
#define DUMMYUNIONNAME2	u2
#define DUMMYUNIONNAME3	u3
#define DUMMYUNIONNAME4	u4
#define DUMMYUNIONNAME5	u5
#define DUMMYUNIONNAME6	u6
#define DUMMYUNIONNAME7	u7
#define DUMMYUNIONNAME8	u8
#else
#define _UNION_NAME(x)
#define DUMMYUNIONNAME
#define DUMMYUNIONNAME2
#define DUMMYUNIONNAME3
#define DUMMYUNIONNAME4
#define DUMMYUNIONNAME5
#define DUMMYUNIONNAME6
#define DUMMYUNIONNAME7
#define DUMMYUNIONNAME8
#endif
#ifndef _ANONYMOUS_STRUCT
#define _ANONYMOUS_STRUCT
#define _STRUCT_NAME(x) x
#define DUMMYSTRUCTNAME	s
#define DUMMYSTRUCTNAME2 s2
#define DUMMYSTRUCTNAME3 s3
#else
#define _STRUCT_NAME(x)
#define DUMMYSTRUCTNAME
#define DUMMYSTRUCTNAME2
#define DUMMYSTRUCTNAME3
#endif

#ifndef NO_STRICT
#ifndef STRICT
#define STRICT 1
#endif
#endif

/* FIXME: This will make some code compile. The programs will most
   likely crash when an exception is raised, but at least they will
   compile. */
#if defined (__GNUC__) && defined (__SEH_NOOP)
#define __try
#define __except(x) if (0) /* don't execute handler */
#define __finally

#define _try __try
#define _except __except
#define _finally __finally
#endif

/**
 * Unsigned 32-bit integer (unsigned long). The Win32 API's general-purpose
 * unsigned type: sizes, flags, times in milliseconds and error codes.
 *
 * long is 32 bits on every grimace target, so DWORD is the same size as UINT
 * but a different type for overloading and printf (%lu).
 */
typedef unsigned long DWORD;
/**
 * The int behind BOOL, from CeGCC's w32api.
 *
 * BOOL is a typedef of it, except under Objective-C, where BOOL is a macro for
 * WINBOOL. Use BOOL in ordinary code.
 */
typedef int WINBOOL;
typedef int *PWINBOOL;
typedef int *LPWINBOOL;
/* FIXME: Is there a good solution to this? */
#ifndef XFree86Server
#ifndef __OBJC__
typedef WINBOOL BOOL;
#else
#define BOOL WINBOOL
#endif
/**
 * Unsigned 8-bit integer (unsigned char).
 */
typedef unsigned char BYTE;
#endif /* ndef XFree86Server */
/**
 * Pointer to a BOOL.
 *
 * See BOOL: Boolean result or flag: an int holding TRUE (1) or FALSE (0).
 */
typedef BOOL *PBOOL;
/**
 * Pointer to BOOL: a flag the callee sets, such as WideCharToMultiByte's "used
 * default char".
 */
typedef BOOL *LPBOOL;
/**
 * Unsigned 16-bit integer (unsigned short). LOWORD, HIWORD and MAKEWORD split
 * and build one.
 */
typedef unsigned short WORD;
/**
 * 32-bit IEEE single precision float.
 *
 * The MIPS and ARM targets are built with soft float, so float arithmetic is
 * done by compiler-rt. The runtime's printf prints floats with three exponent
 * digits (1e+006), as Microsoft's does.
 */
typedef float FLOAT;
/**
 * Pointer to a FLOAT.
 *
 * See FLOAT: 32-bit IEEE single precision float.
 */
typedef FLOAT *PFLOAT;
/**
 * Pointer to a BYTE.
 *
 * See BYTE: Unsigned 8-bit integer (unsigned char).
 */
typedef BYTE *PBYTE;
/**
 * Pointer to BYTE: a byte buffer, as in RegQueryValueExW.
 */
typedef BYTE *LPBYTE;
typedef int *PINT;
/**
 * Pointer to int: an integer the callee fills in, such as a character count or
 * an array of widths.
 */
typedef int *LPINT;
/**
 * Pointer to a WORD.
 *
 * See WORD: Unsigned 16-bit integer (unsigned short).
 */
typedef WORD *PWORD;
/**
 * Pointer to WORD: a 16-bit value the callee fills in.
 */
typedef WORD *LPWORD;
/**
 * Pointer to long: a 32-bit signed value the callee fills in or, for the
 * Interlocked functions, operates on.
 */
typedef long *LPLONG;
/**
 * Pointer to a DWORD.
 *
 * See DWORD: Unsigned 32-bit integer (unsigned long).
 */
typedef DWORD *PDWORD;
/**
 * Pointer to DWORD: a 32-bit value the callee fills in, such as a byte count
 * or a size on input and output.
 */
typedef DWORD *LPDWORD;
typedef CONST void *PCVOID;
/**
 * Pointer to read-only memory of any type (const void *): a buffer the callee
 * only reads, as in WriteFile.
 */
typedef CONST void *LPCVOID;
/**
 * Signed 32-bit integer (int).
 */
typedef int INT;
typedef unsigned int UINT,*PUINT,*LPUINT;

#include <winnt.h>

/**
 * First message parameter, passed to window procedures and SendMessageW.
 *
 * A typedef of UINT_PTR (unsigned int): 32 bits, wide enough for a pointer or
 * a handle as well as a small integer. What it holds depends on the message.
 */
typedef UINT_PTR WPARAM;
/**
 * Second message parameter, passed to window procedures and SendMessageW.
 *
 * A typedef of LONG_PTR (long): 32 bits, so it carries a pointer or handle as
 * often as an integer. Window and timer callbacks also receive one as the
 * caller-supplied value.
 */
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;
#ifndef _HRESULT_DEFINED
/**
 * Status code returned by COM, OLE and shell functions: a 32-bit LONG.
 *
 * Zero (S_OK) and other non-negative values are success; negative values are
 * failures, with the facility and code in the low bits. Test with SUCCEEDED
 * and FAILED, not against S_OK, since functions may return S_FALSE and other
 * non-zero successes. COM arrives on CE with the 3.0 SDK headers.
 */
typedef LONG HRESULT;
#define _HRESULT_DEFINED
#endif
#ifndef XFree86Server
/**
 * 16-bit identifier for a string in an atom table (WORD).
 *
 * On Windows CE the only ATOMs a program sees come from RegisterClassW, which
 * returns one for the registered class. CE 1.0 to 3.0 ROMs export no atom
 * table functions; GlobalAddAtomW and the rest arrive with CE 4.1.
 */
typedef WORD ATOM;
#endif /* XFree86Server */
/**
 * Same as HANDLE.
 *
 * See HANDLE: Opaque handle to a kernel object: a file, event, mutex, semaphore, process, thread, find context or similar.
 */
typedef HANDLE HHOOK;
/**
 * Handle to a memory block from the global heap.
 *
 * A typedef of HANDLE. Windows CE has no GlobalAlloc or GlobalFree: where a
 * desktop API wants an HGLOBAL, such as SetClipboardData, pass a block from
 * LocalAlloc. CE memory is always fixed, so the handle is the block's address.
 */
typedef HANDLE HGLOBAL;
/**
 * Handle to a memory block from LocalAlloc.
 *
 * A typedef of HANDLE. Windows CE only supports fixed memory, so the handle is
 * a pointer to the block and LocalLock is a no-op. Free with LocalFree.
 */
typedef HANDLE HLOCAL;
typedef HANDLE GLOBALHANDLE;
typedef HANDLE LOCALHANDLE;
/**
 * Handle to any GDI object: a bitmap, brush, font, pen, region or palette.
 *
 * It's a plain void *, so every GDI handle type converts to it without a cast;
 * SelectObject, DeleteObject and GetObjectW take one. Cast the result of
 * SelectObject back to the specific type.
 */
typedef void *HGDIOBJ;
/**
 * Handle to an accelerator table, from LoadAcceleratorsW or
 * CreateAcceleratorTableW.
 *
 * Pass it to TranslateAcceleratorW in the message loop. Free tables from
 * CreateAcceleratorTableW with DestroyAcceleratorTable; loaded ones are freed
 * when the program exits.
 */
typedef struct HACCEL__ { int i; } *HACCEL;
/**
 * Handle to a bitmap, from CreateBitmap, CreateDIBSection, LoadImageW or
 * LoadBitmapW.
 *
 * Select it into a memory DC to draw on it or BitBlt from it. A GDI object:
 * free with DeleteObject once it's no longer selected into any DC.
 */
typedef struct HBITMAP__ { int i; } *HBITMAP;
/**
 * Handle to a brush, from CreateSolidBrush, CreatePatternBrush, GetStockObject
 * or GetSysColorBrush.
 *
 * A GDI object: free brushes you created with DeleteObject. Stock and system
 * colour brushes are shared; don't delete them. CreatePatternBrush needs CE
 * 2.0 or later.
 */
typedef struct HBRUSH__ { int i; } *HBRUSH;
/**
 * Handle to a colour space, from CreateColorSpaceW. No Windows CE ROM
 * exports colour management; source compatibility only.
 */
typedef struct HCOLORSPACE__ { int i; } *HCOLORSPACE;
/**
 * Handle to a device context: the drawing target and state for GDI calls.
 *
 * A DC is not a GDI object: release one from GetDC or BeginPaint with
 * ReleaseDC or EndPaint, and delete one from CreateCompatibleDC or CreateDCW
 * with DeleteDC. Objects selected into it are freed separately with
 * DeleteObject.
 */
typedef struct HDC__ { int i; } *HDC;
/**
 * Handle to an OpenGL rendering context, from wglCreateContext. No Windows
 * CE ROM exports the wgl functions; source compatibility only.
 */
typedef struct HGLRC__ { int i; } *HGLRC;
/**
 * Handle to an NT desktop. Windows CE has one; no ROM exports the desktop
 * functions. Source compatibility only.
 */
typedef struct HDESK__ { int i; } *HDESK;
/**
 * Handle to an enhanced metafile, from CreateEnhMetaFileW's CloseEnhMetaFile
 * (CE 2.0 and later; 2.11 on ARM).
 *
 * Draw it with PlayEnhMetaFile and free it with DeleteEnhMetaFile.
 */
typedef struct HENHMETAFILE__ { int i; } *HENHMETAFILE;
/**
 * Handle to a logical font, from CreateFontIndirectW.
 *
 * Select it into a DC to draw text with it. A GDI object: free with
 * DeleteObject once no DC has it selected. Windows CE has no CreateFontW; fill
 * in a LOGFONTW instead.
 */
typedef struct HFONT__ { int i; } *HFONT;
/**
 * Handle to an icon, from LoadIconW, LoadImageW or CreateIconIndirect.
 *
 * Free icons you created or loaded with LoadImageW using DestroyIcon. Icons
 * from LoadIconW are shared and need no freeing. HCURSOR is the same type.
 */
typedef struct HICON__ { int i; } *HICON;
/**
 * Handle to an open registry key, from RegOpenKeyExW or RegCreateKeyExW, or
 * one of the predefined roots (HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER,
 * HKEY_CLASSES_ROOT).
 *
 * Close opened keys with RegCloseKey, not CloseHandle.
 */
typedef struct HKEY__ { int i; } *HKEY;
/* FIXME: How to handle these. SM_CMONITORS etc in winuser.h also. */
/**
 * Handle to a display monitor, from MonitorFromWindow, MonitorFromPoint or
 * EnumDisplayMonitors (CE 4.1 and later; 4.2 on MIPS).
 *
 * Earlier ROMs export none of the monitor functions: the one display is
 * described by GetSystemMetrics and GetDeviceCaps.
 */
typedef struct HMONITOR__ { int i; } *HMONITOR;
#define HMONITOR_DECLARED 1
/**
 * Handle to a terminal, an NT internal. Nothing in Windows CE uses it;
 * source compatibility only.
 */
typedef struct HTERMINAL__ { int i; } *HTERMINAL;
/**
 * Handle to a window event hook, from SetWinEventHook. No Windows CE ROM
 * exports it; source compatibility only.
 */
typedef struct HWINEVENTHOOK__ { int i; } *HWINEVENTHOOK;

/**
 * Pointer to a HKEY.
 *
 * See HKEY: Handle to an open registry key, from RegOpenKeyExW or RegCreateKeyExW, or one of the predefined roots (HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER, HKEY_CLASSES_ROOT).
 */
typedef HKEY *PHKEY;
/**
 * Handle to a menu, from CreateMenu, CreatePopupMenu or LoadMenuW.
 *
 * Free with DestroyMenu unless it's been attached to a window or command bar,
 * which destroys it. Windows CE windows have no menu bar: show a menu on a
 * command bar with CommandBar_InsertMenubar, or as a popup with
 * TrackPopupMenuEx (there's no TrackPopupMenu).
 */
typedef struct HMENU__ { int i; } *HMENU;
/**
 * Handle to a Windows 3.x format metafile. No Windows CE ROM exports the
 * old metafile functions; use enhanced metafiles (HENHMETAFILE). Source
 * compatibility only.
 */
typedef struct HMETAFILE__ { int i; } *HMETAFILE;
/**
 * Handle to a loaded module: a program's own, as passed to WinMain, or a DLL
 * from LoadLibraryW.
 *
 * HMODULE is the same type. Pass it to resource functions (LoadIconW,
 * FindResource, LoadStringW) and RegisterClassW. GetModuleHandleW needs
 * Windows CE 2.0 or later.
 */
typedef struct HINSTANCE__ { int i; } *HINSTANCE;
/**
 * Handle to a loaded module, from LoadLibraryW or GetModuleHandleW.
 *
 * A typedef of HINSTANCE: the two are interchangeable. Look up exports with
 * GetProcAddressW and release with FreeLibrary, not CloseHandle.
 */
typedef HINSTANCE HMODULE;
/**
 * Handle to a logical palette, from CreatePalette (CE 2.0 and later).
 *
 * Apply with SelectPalette and RealizePalette. A GDI object: free with
 * DeleteObject. Windows CE palettes hold exactly the entries you supply; the
 * system colours aren't added.
 */
typedef struct HPALETTE__ { int i; } *HPALETTE;
/**
 * Handle to a pen, from CreatePenIndirect, CreatePen or GetStockObject.
 *
 * A GDI object: free pens you created with DeleteObject. CE 1.0 exports
 * CreatePenIndirect but not CreatePen.
 */
typedef struct HPEN__ { int i; } *HPEN;
/**
 * Handle to a region, from CreateRectRgn, CreateRectRgnIndirect or CombineRgn.
 *
 * A GDI object: free with DeleteObject. CE 1.0 exports CreateRectRgnIndirect
 * but not CreateRectRgn.
 */
typedef struct HRGN__ { int i; } *HRGN;
/**
 * Handle to a resource located by FindResource (CE exports it without the W;
 * FindResourceW is the same function).
 *
 * Pass it to LoadResource and SizeofResource. It identifies the resource
 * entry, not its data, and isn't freed.
 */
typedef struct HRSRC__ { int i; } *HRSRC;
/**
 * Handle to a string resource in Win16. Nothing in Win32 or Windows CE uses
 * it; source compatibility only.
 */
typedef struct HSTR__ { int i; } *HSTR;
/**
 * Handle to a Win16 task. Nothing in Win32 or Windows CE uses it; source
 * compatibility only.
 */
typedef struct HTASK__ { int i; } *HTASK;
/**
 * Handle to a window, from CreateWindowExW, FindWindowW, GetDlgItem and the
 * like.
 *
 * STRICT is on, so HWND is a distinct pointer type (struct HWND__ *) and can't
 * be mixed with other handles without a cast. Destroy with DestroyWindow.
 */
typedef struct HWND__ { int i; } *HWND;
/**
 * Handle to an NT window station. Windows CE has one; no ROM exports the
 * window station functions. Source compatibility only.
 */
typedef struct HWINSTA__ { int i; } *HWINSTA;
/**
 * Handle to a keyboard layout, from GetKeyboardLayout (CE 2.11 and later)
 * or LoadKeyboardLayoutW and ActivateKeyboardLayout (CE 4.2 and later).
 *
 * Also the type of WM_INPUTLANGCHANGE's lParam. A layout isn't freed.
 */
typedef struct HKL__ { int i; } *HKL;
typedef int HFILE;
/**
 * Handle to a cursor, from LoadCursorW.
 *
 * A typedef of HICON. Most Windows CE devices have no mouse cursor and support
 * only the wait cursor, IDC_WAIT, shown with SetCursor and hidden with
 * SetCursor(NULL). Devices with a cursor load monochrome cursors only.
 * DestroyCursor needs CE 3.0 or later; cursors from LoadCursorW are shared and
 * aren't destroyed.
 */
typedef HICON HCURSOR;
/**
 * A colour as 0x00BBGGRR in a DWORD. Build with RGB and read with GetRValue,
 * GetGValue and GetBValue. CLR_INVALID (0xFFFFFFFF) is the failure value of
 * functions that return one.
 */
typedef DWORD COLORREF;
/**
 * Generic function pointer: int (WINAPI *)() with no prototype. What
 * GetProcAddressW returns.
 *
 * Cast it to the exported function's real pointer type before calling it;
 * calling through a FARPROC directly passes arguments by the unprototyped
 * rules. FAR means nothing on Win32; PROC is the same type.
 *
 * @return Meaningless until cast.
 */
typedef int (WINAPI *FARPROC)();
typedef int (WINAPI *NEARPROC)();
/**
 * Generic function pointer, the same type as FARPROC: int (WINAPI *)() with no
 * prototype. Cast it to the real function type before calling.
 *
 * @return Meaningless until cast.
 */
typedef int (WINAPI *PROC)();
/**
 * This structure defines the coordinates of the upper-left and lower-right
 * corners of a rectangle.
 *
 * When RECT is passed to the FillRect function, the rectangle is filled up
 * to, but not including, the right column and bottom row of pixels. This
 * structure is identical to the RECTL structure.
 */
typedef struct tagRECT {
	/**
	 * Specifies the x-coordinate of the upper-left corner of the
	 * rectangle.
	 */
	LONG left;
	/**
	 * Specifies the y-coordinate of the upper-left corner of the
	 * rectangle.
	 */
	LONG top;
	/**
	 * Specifies the x-coordinate of the lower-right corner of
	 * the rectangle.
	 */
	LONG right;
	/**
	 * Specifies the y-coordinate of the lower-right corner of
	 * the rectangle.
	 */
	LONG bottom;
} RECT,*PRECT;
/**
 * Pointer to a RECT the callee reads or fills in, as in GetClientRect.
 */
typedef RECT *LPRECT;
/**
 * Pointer to a read-only RECT, the rectangle given by its left, top, right and
 * bottom edges.
 */
typedef const RECT *LPCRECT;
typedef struct tagRECTL {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECTL,*PRECTL,*LPRECTL;
typedef const RECTL *LPCRECTL;
/**
 * This structure defines the x- and y-coordinates of a point.
 */
typedef struct tagPOINT {
	/**
	 * Specifies the x-coordinate of the point.
	 */
	LONG x;
	/**
	 * Specifies the y-coordinate of the point.
	 */
	LONG y;
} POINT,POINTL,*PPOINTL,*LPPOINTL;
/**
 * Pointer to a POINT.
 *
 * See POINT: This structure defines the x- and y-coordinates of a point.
 */
typedef POINT *PPOINT;
/**
 * Pointer to a POINT, the x and y of a position, as filled by GetCursorPos or
 * converted by ClientToScreen.
 */
typedef POINT *LPPOINT;
/**
 * This structure specifies the width and height of a rectangle.
 *
 * The rectangle dimensions stored in this structure may correspond to
 * viewport extents, window extents, text extents, bitmap dimensions, or
 * the aspect-ratio filter for some extended functions.
 */
typedef struct tagSIZE {
	/**
	 * Specifies the rectangle's width.
	 */
	LONG cx;
	/**
	 * Specifies the rectangle's height.
	 */
	LONG cy;
} SIZE,SIZEL,*PSIZE,*PSIZEL,*LPSIZEL;
/**
 * Pointer to a SIZE.
 *
 * See SIZE: This structure specifies the width and height of a rectangle.
 */
typedef SIZE *LPSIZE;
/**
 * This structure defines the coordinates of a point.
 */
typedef struct tagPOINTS {
	/**
	 * Specifies the x-coordinate of the point.
	 */
	SHORT x;
	/**
	 * Specifies the y-coordinate of the point.
	 */
	SHORT y;
} POINTS,*PPOINTS,*LPPOINTS;

#ifdef __cplusplus
}
#endif
#endif
