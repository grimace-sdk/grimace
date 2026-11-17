#ifndef _BASETSD_H
#define _BASETSD_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifdef __GNUC__
#ifndef __int64
#define __int64 long long
#endif
#endif

#if defined(_WIN64)
#define __int3264   __int64
#define ADDRESS_TAG_BIT 0x40000000000UI64
#else /*  !_WIN64 */
#define __int3264   __int32
#define ADDRESS_TAG_BIT 0x80000000UL
#define HandleToUlong( h ) ((ULONG)(ULONG_PTR)(h) )
#define HandleToLong( h ) ((LONG)(LONG_PTR) (h) )
#define LongToHandle( h) ((HANDLE)(LONG_PTR) (h))
#define PtrToUlong( p ) ((ULONG)(ULONG_PTR) (p) )
#define PtrToLong( p ) ((LONG)(LONG_PTR) (p) )
#define PtrToUint( p ) ((UINT)(UINT_PTR) (p) )
#define PtrToInt( p ) ((INT)(INT_PTR) (p) )
#define PtrToUshort( p ) ((unsigned short)(ULONG_PTR)(p) )
#define PtrToShort( p ) ((short)(LONG_PTR)(p) )
#define IntToPtr( i )    ((VOID*)(INT_PTR)((int)i))
#define UIntToPtr( ui )  ((VOID*)(UINT_PTR)((unsigned int)ui))
#define LongToPtr( l )   ((VOID*)(LONG_PTR)((long)l))
#define ULongToPtr( ul )  ((VOID*)(ULONG_PTR)((unsigned long)ul))
#endif /* !_WIN64 */

#define UlongToPtr(ul) ULongToPtr(ul)
#define UintToPtr(ui) UIntToPtr(ui)
#define MAXUINT_PTR  (~((UINT_PTR)0))
#define MAXINT_PTR   ((INT_PTR)(MAXUINT_PTR >> 1))
#define MININT_PTR   (~MAXINT_PTR)
#define MAXULONG_PTR (~((ULONG_PTR)0))
#define MAXLONG_PTR  ((LONG_PTR)(MAXULONG_PTR >> 1))
#define MINLONG_PTR  (~MAXLONG_PTR)
#define MAXUHALF_PTR ((UHALF_PTR)~0)
#define MAXHALF_PTR  ((HALF_PTR)(MAXUHALF_PTR >> 1))
#define MINHALF_PTR  (~MAXHALF_PTR)

#ifndef RC_INVOKED
#ifdef __cplusplus
extern "C" {
#endif
typedef signed char INT8;
typedef signed short INT16;
typedef int LONG32, *PLONG32;
#ifndef XFree86Server
typedef int INT32, *PINT32;
#endif /* ndef XFree86Server */
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int ULONG32, *PULONG32;
typedef unsigned int DWORD32, *PDWORD32;
typedef unsigned int UINT32, *PUINT32;

#if defined(_WIN64)
/**
 * Signed integer the size of a pointer: int, 32 bits here. Dialog procedures
 * on desktop Windows return it; on these headers DLGPROC returns BOOL.
 */
typedef __int64 INT_PTR;
typedef __int64 *PINT_PTR;
/**
 * Unsigned integer the size of a pointer: unsigned int, 32 bits here. WPARAM
 * is a typedef of it.
 */
typedef unsigned __int64 UINT_PTR;
typedef unsigned __int64 *PUINT_PTR;
/**
 * Signed integer the size of a pointer: long, 32 bits on every grimace target.
 *
 * The _PTR types exist so code also compiles for 64-bit Windows; here they are
 * plain 32-bit integers. LPARAM and LRESULT are typedefs of it.
 */
typedef __int64 LONG_PTR;
typedef __int64 *PLONG_PTR;
/**
 * Unsigned integer the size of a pointer: unsigned long, 32 bits here. SIZE_T
 * and DWORD_PTR are typedefs of it.
 */
typedef unsigned __int64 ULONG_PTR;
typedef unsigned __int64 *PULONG_PTR;
typedef unsigned __int64 HANDLE_PTR;
typedef unsigned int UHALF_PTR, *PUHALF_PTR;
typedef int HALF_PTR, *PHALF_PTR;

#if 0 /* TODO when WIN64 is here */
inline unsigned long HandleToUlong(const void* h )
    { return((unsigned long) h ); }
inline long HandleToLong( const void* h )
    { return((long) h ); }
inline void* LongToHandle( const long h )
    { return((void*) (INT_PTR) h ); }
inline unsigned long PtrToUlong( const void* p)
    { return((unsigned long) p ); }
inline unsigned int PtrToUint( const void* p )
    { return((unsigned int) p ); }
inline unsigned short PtrToUshort( const void* p )
    { return((unsigned short) p ); }
inline long PtrToLong( const void* p )
    { return((long) p ); }
inline int PtrToInt( const void* p )
    { return((int) p ); }
inline short PtrToShort( const void* p )
    { return((short) p ); }
inline void* IntToPtr( const int i )
    { return( (void*)(INT_PTR)i ); }
inline void* UIntToPtr(const unsigned int ui)
    { return( (void*)(UINT_PTR)ui ); }
inline void* LongToPtr( const long l )
    { return( (void*)(LONG_PTR)l ); }
inline void* ULongToPtr( const unsigned long ul )
    { return( (void*)(ULONG_PTR)ul ); }
#endif /* 0_ */

#else /*  !_WIN64 */
typedef  int INT_PTR, *PINT_PTR;
typedef  unsigned int UINT_PTR, *PUINT_PTR;
typedef  long LONG_PTR, *PLONG_PTR;
typedef  unsigned long ULONG_PTR, *PULONG_PTR;
typedef unsigned short UHALF_PTR, *PUHALF_PTR;
typedef short HALF_PTR, *PHALF_PTR;
typedef unsigned long HANDLE_PTR;
#endif /* !_WIN64 */

/**
 * Unsigned byte count the size of a pointer: a typedef of ULONG_PTR (unsigned
 * long), 32 bits here.
 *
 * Not the same type as the C library's size_t, which clang defines as unsigned
 * int on these targets: the same size, but distinct for overloading and
 * printf. Use %lu for SIZE_T and %u for size_t; the runtime's printf has no
 * %zu.
 */
typedef ULONG_PTR SIZE_T;
/**
 * Pointer to a ULONG_PTR.
 *
 * See ULONG_PTR: Unsigned integer the size of a pointer: unsigned long, 32 bits here.
 */
typedef ULONG_PTR *PSIZE_T;
/**
 * Signed byte count the size of a pointer: a typedef of LONG_PTR (long), 32
 * bits here.
 */
typedef LONG_PTR SSIZE_T;
typedef LONG_PTR *PSSIZE_T;
/**
 * Unsigned integer the size of a pointer: a typedef of ULONG_PTR, so unsigned
 * long and 32 bits here. Used for values that may hold either a DWORD or a
 * pointer.
 */
typedef ULONG_PTR DWORD_PTR;
/**
 * Pointer to a ULONG_PTR.
 *
 * See ULONG_PTR: Unsigned integer the size of a pointer: unsigned long, 32 bits here.
 */
typedef ULONG_PTR *PDWORD_PTR;
typedef __int64 LONG64, *PLONG64;
typedef __int64 INT64,  *PINT64;
typedef unsigned __int64 ULONG64, *PULONG64;
typedef unsigned __int64 DWORD64, *PDWORD64;
typedef unsigned __int64 UINT64,  *PUINT64;
#ifdef __cplusplus
}
#endif
#endif /* !RC_INVOKED */

#endif /* _BASETSD_H */
