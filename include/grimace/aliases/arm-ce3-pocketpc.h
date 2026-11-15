#ifdef _MSACM_H
#undef acmDriverAdd
extern __typeof__(acmDriverAddW) acmDriverAdd;
#undef acmDriverAddW
#define acmDriverAddW acmDriverAdd
#undef acmDriverDetails
extern __typeof__(acmDriverDetailsW) acmDriverDetails;
#undef acmDriverDetailsW
#define acmDriverDetailsW acmDriverDetails
#undef acmFormatEnum
extern __typeof__(acmFormatEnumW) acmFormatEnum;
#undef acmFormatEnumW
#define acmFormatEnumW acmFormatEnum
#undef acmFormatTagEnum
extern __typeof__(acmFormatTagEnumW) acmFormatTagEnum;
#undef acmFormatTagEnumW
#define acmFormatTagEnumW acmFormatTagEnum
#endif
#ifdef _RAS_H
#undef RasDeleteEntry
extern __typeof__(RasDeleteEntryW) RasDeleteEntry;
#undef RasDeleteEntryW
#define RasDeleteEntryW RasDeleteEntry
#undef RasDial
extern __typeof__(RasDialW) RasDial;
#undef RasDialW
#define RasDialW RasDial
#undef RasEnumConnections
extern __typeof__(RasEnumConnectionsW) RasEnumConnections;
#undef RasEnumConnectionsW
#define RasEnumConnectionsW RasEnumConnections
#undef RasEnumEntries
extern __typeof__(RasEnumEntriesW) RasEnumEntries;
#undef RasEnumEntriesW
#define RasEnumEntriesW RasEnumEntries
#undef RasGetConnectStatus
extern __typeof__(RasGetConnectStatusW) RasGetConnectStatus;
#undef RasGetConnectStatusW
#define RasGetConnectStatusW RasGetConnectStatus
#undef RasGetEntryDialParams
extern __typeof__(RasGetEntryDialParamsW) RasGetEntryDialParams;
#undef RasGetEntryDialParamsW
#define RasGetEntryDialParamsW RasGetEntryDialParams
#undef RasGetEntryProperties
extern __typeof__(RasGetEntryPropertiesW) RasGetEntryProperties;
#undef RasGetEntryPropertiesW
#define RasGetEntryPropertiesW RasGetEntryProperties
#undef RasHangUp
extern __typeof__(RasHangUpW) RasHangUp;
#undef RasHangUpW
#define RasHangUpW RasHangUp
#undef RasRenameEntry
extern __typeof__(RasRenameEntryW) RasRenameEntry;
#undef RasRenameEntryW
#define RasRenameEntryW RasRenameEntry
#undef RasSetEntryDialParams
extern __typeof__(RasSetEntryDialParamsW) RasSetEntryDialParams;
#undef RasSetEntryDialParamsW
#define RasSetEntryDialParamsW RasSetEntryDialParams
#undef RasSetEntryProperties
extern __typeof__(RasSetEntryPropertiesW) RasSetEntryProperties;
#undef RasSetEntryPropertiesW
#define RasSetEntryPropertiesW RasSetEntryProperties
#undef RasValidateEntryName
extern __typeof__(RasValidateEntryNameW) RasValidateEntryName;
#undef RasValidateEntryNameW
#define RasValidateEntryNameW RasValidateEntryName
#endif
#ifdef _WINBASE_H
#undef GetSystemDirectory
extern __typeof__(GetSystemDirectoryW) GetSystemDirectory;
#undef GetSystemDirectoryW
#define GetSystemDirectoryW GetSystemDirectory
#undef GetWindowsDirectory
extern __typeof__(GetWindowsDirectoryW) GetWindowsDirectory;
#undef GetWindowsDirectoryW
#define GetWindowsDirectoryW GetWindowsDirectory
#endif
#ifdef _WINREG_H
#undef RegReplaceKey
extern __typeof__(RegReplaceKeyW) RegReplaceKey;
#undef RegReplaceKeyW
#define RegReplaceKeyW RegReplaceKey
#undef RegSaveKey
extern __typeof__(RegSaveKeyW) RegSaveKey;
#undef RegSaveKeyW
#define RegSaveKeyW RegSaveKey
#endif
#ifdef _WINUSER_H
#undef CopyAcceleratorTable
extern __typeof__(CopyAcceleratorTableW) CopyAcceleratorTable;
#undef CopyAcceleratorTableW
#define CopyAcceleratorTableW CopyAcceleratorTable
#undef EnumPropsEx
extern __typeof__(EnumPropsExW) EnumPropsEx;
#undef EnumPropsExW
#define EnumPropsExW EnumPropsEx
#undef GetMonitorInfo
extern __typeof__(GetMonitorInfoW) GetMonitorInfo;
#undef GetMonitorInfoW
#define GetMonitorInfoW GetMonitorInfo
#undef SendMessageTimeout
extern __typeof__(SendMessageTimeoutW) SendMessageTimeout;
#undef SendMessageTimeoutW
#define SendMessageTimeoutW SendMessageTimeout
#endif
