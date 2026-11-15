#ifndef __DBGAPI_H_
#define __DBGAPI_H_

#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#if _WIN32_WCE >= 0x0200

/**
 * Writes a formatted message to the kernel's debug output.
 *
 * DEBUGMSG, RETAILMSG and the other dbgapi.h macros call it. Where the
 * output goes depends on the device: usually a debug serial port or a
 * kernel debugger.
 *
 * @param lpszFmt Wide-character printf-style format string, followed by
 *        its arguments.
 *
 * @note Windows CE 2.0 and later.
 */
void NKDbgPrintfW(LPCWSTR lpszFmt, ...);

#define RETAILMSG(cond,printf_exp) ((void)((cond)?(NKDbgPrintfW printf_exp),1:0))
#define ERRORMSG(cond,printf_exp) ((void)((cond)?(NKDbgPrintfW(L"ERROR: %hs line %d: ",__FILE__,__LINE__),NKDbgPrintfW printf_exp),1:0))
#define RETAILREGISTERZONES(x) ((void)0)
#define DEBUGREGISTER(x) ((void)0)

#if _WIN32_WCE < 0x0300 || defined(GRIMACE_NO_SDK)
typedef struct _DBGPARAM {
	WCHAR lpszName[32];
	WCHAR rglpszZones[16][32];
	ULONG ulZoneMask;
} DBGPARAM, *LPDBGPARAM;
#endif

#ifdef DEBUG
extern struct _DBGPARAM dpCurSettings;
#define DEBUGZONE(n) (dpCurSettings.ulZoneMask&(0x00000001<<(n)))
#define DEBUGMSG(cond,printf_exp) ((void)((cond)?(NKDbgPrintfW printf_exp),1:0))
#define DEBUGCHK(exp) ((void)((exp)?1:(NKDbgPrintfW(L"DEBUGCHK failed in %hs at line %d\r\n",__FILE__,__LINE__),0)))
#else
#define DEBUGZONE(n) 0
#define DEBUGMSG(cond,printf_exp) ((void)0)
#define DEBUGCHK(exp) ((void)0)
#endif
#define ASSERT(exp) DEBUGCHK(exp)

#endif	/* _WIN32_WCE */

#ifdef  __cplusplus
}
#endif

#endif	/* __DBGAPI_H_ */
