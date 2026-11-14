#ifndef GRIMACE_AYGSHELL_H
#define GRIMACE_AYGSHELL_H

#include <windows.h>
#include <sipapi.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SHFS_SHOWTASKBAR 0x0001
#define SHFS_HIDETASKBAR 0x0002
#define SHFS_SHOWSIPBUTTON 0x0004
#define SHFS_HIDESIPBUTTON 0x0008
#define SHFS_SHOWSTARTICON 0x0010
#define SHFS_HIDESTARTICON 0x0020

BOOL WINAPI SHFullScreen(HWND hwndRequester, DWORD dwState);

#define SHIDIM_FLAGS 0x0001

#define SHIDIF_DONEBUTTON 0x0001
#define SHIDIF_SIZEDLG 0x0002
#define SHIDIF_SIZEDLGFULLSCREEN 0x0004
#define SHIDIF_SIPDOWN 0x0008
#define SHIDIF_FULLSCREENNOMENUBAR 0x0010
#define SHIDIF_EMPTYMENU 0x0020
#define SHIDIF_WANTSCROLLBAR 0x0040

typedef struct tagSHINITDLGINFO {
    DWORD dwMask;
    HWND hDlg;
    DWORD dwFlags;
} SHINITDLGINFO, *PSHINITDLGINFO;

BOOL WINAPI SHInitDialog(PSHINITDLGINFO pshidi);

#define SHDB_SHOW 0x0001
#define SHDB_HIDE 0x0002
#define SHDB_SHOWCANCEL 0x0004

BOOL WINAPI SHDoneButton(HWND hwndRequester, DWORD dwState);

typedef struct tagSHACTIVATEINFO {
    DWORD cbSize;
    HWND hwndLastFocus;
    UINT fSipUp : 1;
    UINT fSipOnDeactivation : 1;
    UINT fActive : 1;
    UINT fReserved : 29;
} SHACTIVATEINFO, *PSHACTIVATEINFO;

#define SHA_INPUTDIALOG 0x00000001

BOOL WINAPI SHHandleWMActivate(HWND hwnd, WPARAM wParam, LPARAM lParam, SHACTIVATEINFO *psai, DWORD dwFlags);
BOOL WINAPI SHHandleWMSettingChange(HWND hwnd, WPARAM wParam, LPARAM lParam, SHACTIVATEINFO *psai);

typedef enum tagSIPSTATE {
    SIP_UP = 0,
    SIP_DOWN,
    SIP_FORCEDOWN,
    SIP_UNCHANGED,
    SIP_INPUTDIALOG
} SIPSTATE;

BOOL WINAPI SHSipPreference(HWND hwnd, SIPSTATE st);

#define SHCMBF_EMPTYBAR 0x0001
#define SHCMBF_HIDDEN 0x0002
#define SHCMBF_HIDESIPBUTTON 0x0004
#define SHCMBF_COLORBK 0x0008
#define SHCMBF_HMENU 0x0010

typedef struct tagSHMENUBARINFO {
    DWORD cbSize;
    HWND hwndParent;
    DWORD dwFlags;
    UINT nToolBarId;
    HINSTANCE hInstRes;
    int nBmpId;
    int cBmpImages;
    HWND hwndMB;
    COLORREF clrBk;
} SHMENUBARINFO, *PSHMENUBARINFO;

BOOL WINAPI SHCreateMenuBar(SHMENUBARINFO *pmbi);

void WINAPI SHIdleTimerReset(void);
HBITMAP WINAPI SHLoadDIBitmap(LPCWSTR szFileName);

#ifdef __cplusplus
}
#endif

#endif
