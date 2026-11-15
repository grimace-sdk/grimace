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
