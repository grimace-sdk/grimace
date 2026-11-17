#ifndef GRIMACE_PM_H
#define GRIMACE_PM_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PBT_TRANSITION 0x00000001
#define PBT_RESUME 0x00000002
#define PBT_POWERSTATUSCHANGE 0x00000004
#define PBT_POWERINFOCHANGE 0x00000008
#define PBT_SUSPENDKEYPRESSED 0x00000100
#define PBT_OEMBASE 0x00010000
#define POWER_NOTIFY_ALL 0xFFFFFFFF

#define POWER_STATE_ON 0x00010000
#define POWER_STATE_OFF 0x00020000
#define POWER_STATE_CRITICAL 0x00040000
#define POWER_STATE_BOOT 0x00080000
#define POWER_STATE_IDLE 0x00100000
#define POWER_STATE_SUSPEND 0x00200000
#define POWER_STATE_UNATTENDED 0x00400000
#define POWER_STATE_RESET 0x00800000
#define POWER_STATE_USERIDLE 0x01000000
#define POWER_STATE_BACKLIGHTON 0x02000000
#define POWER_STATE_PASSWORD 0x10000000

typedef struct _POWER_BROADCAST {
    DWORD Message;
    DWORD Flags;
    DWORD Length;
    WCHAR SystemPowerState[1];
} POWER_BROADCAST, *PPOWER_BROADCAST;

/**
 * Subscribes a message queue to power state notifications.
 *
 * Create the queue with CreateMsgQueue for reading. Each notification
 * arrives as a POWER_BROADCAST structure. Stop with
 * StopPowerNotifications. Windows CE 4.1 and later.
 *
 * @param hMsgQ Queue handle.
 * @param Flags PBT_ flags for the notifications wanted, or
 *        POWER_NOTIFY_ALL.
 * @return Notification handle, or NULL on failure.
 */
HANDLE WINAPI RequestPowerNotifications(HANDLE hMsgQ, DWORD Flags);
/**
 * Ends power notifications started with RequestPowerNotifications.
 *
 * Windows CE 4.1 and later.
 *
 * @param h Handle from RequestPowerNotifications.
 * @return ERROR_SUCCESS, or an error code.
 */
DWORD WINAPI StopPowerNotifications(HANDLE h);

typedef enum _CEDEVICE_POWER_STATE {
    PwrDeviceUnspecified = -1,
    D0 = 0,
    D1,
    D2,
    D3,
    D4,
    PwrDeviceMaximum
} CEDEVICE_POWER_STATE, *PCEDEVICE_POWER_STATE;

#define IOCTL_POWER_CAPABILITIES 0x00321000
#define IOCTL_POWER_GET 0x00321004
#define IOCTL_POWER_SET 0x00321008

#define DX_MASK(dx) (0x01 << (dx))

typedef struct _POWER_CAPABILITIES {
    UCHAR DeviceDx;
    UCHAR WakeFromDx;
    UCHAR InrushDx;
    DWORD Power[5];
    DWORD Latency[5];
    DWORD Flags;
} POWER_CAPABILITIES, *PPOWER_CAPABILITIES;

#define POWER_NAME 0x00000001
#define POWER_FORCE 0x00001000

#define PPN_REEVALUATESTATE 0x00000001
#define PPN_POWERCHANGE 0x00000002
#define PPN_UNATTENDEDMODE 0x00000003
#define PPN_SUSPENDKEYPRESSED 0x00000004
#define PPN_POWERBUTTONPRESSED 0x00000004
#define PPN_SUSPENDKEYRELEASED 0x00000005
#define PPN_APPBUTTONPRESSED 0x00000006
#define PPN_OEMBASE 0x00010000

/**
 * Asks the Power Manager to keep a device at or above a power state.
 *
 * Lasts until ReleasePowerRequirement is called or the calling process
 * exits. Windows CE 4.1 and later.
 *
 * @param pvDevice Device name, optionally prefixed with its power class
 *        GUID.
 * @param DeviceState Minimum state, D0 (full on) to D4 (off).
 * @param DeviceFlags POWER_NAME, optionally with POWER_FORCE.
 * @param pvSystemState System power state the requirement applies to, or
 *        NULL for all.
 * @param StateFlags Reserved, pass 0.
 * @return Requirement handle, or NULL on failure.
 */
HANDLE WINAPI SetPowerRequirement(PVOID pvDevice, CEDEVICE_POWER_STATE DeviceState, ULONG DeviceFlags, PVOID pvSystemState, ULONG StateFlags);
/**
 * Cancels a minimum device power state set with SetPowerRequirement.
 *
 * Windows CE 4.1 and later.
 *
 * @param hPowerReq Handle from SetPowerRequirement.
 * @return ERROR_SUCCESS, or an error code.
 */
DWORD WINAPI ReleasePowerRequirement(HANDLE hPowerReq);
/**
 * Asks the Power Manager to move a device to a power state.
 *
 * Called by a driver or application on behalf of a device. The Power
 * Manager decides whether to honour the request and then sets the state
 * through the driver's IOCTL_POWER_SET. Windows CE 4.1 and later.
 *
 * @param pvDevice Device name, optionally prefixed with its power class
 *        GUID, as given by Flags.
 * @param DeviceState Requested state, D0 (full on) to D4 (off).
 * @param Flags POWER_NAME when pvDevice is a name.
 * @return ERROR_SUCCESS, or an error code.
 */
DWORD WINAPI DevicePowerNotify(PVOID pvDevice, CEDEVICE_POWER_STATE DeviceState, DWORD Flags);
/**
 * Gets the name and flags of the current system power state.
 *
 * Windows CE 4.1 and later.
 *
 * @param pBuffer Receives the state name, or NULL.
 * @param dwBufChars Size of pBuffer in characters.
 * @param pdwFlags Receives POWER_STATE_ flags for the state, or NULL.
 * @return ERROR_SUCCESS, or an error code.
 */
DWORD WINAPI GetSystemPowerState(LPWSTR pBuffer, DWORD dwBufChars, PDWORD pdwFlags);
/**
 * Asks the Power Manager to change the system power state.
 *
 * Name the state, or pass NULL and give it as a POWER_STATE_ flag. Windows
 * CE 4.1 and later.
 *
 * @param pwsSystemState Name of the state, or NULL.
 * @param StateFlags POWER_STATE_ flag, such as POWER_STATE_SUSPEND, used
 *        when pwsSystemState is NULL.
 * @param Options POWER_FORCE to skip notifying applications, or 0.
 * @return ERROR_SUCCESS, or an error code.
 */
DWORD WINAPI SetSystemPowerState(LPCWSTR pwsSystemState, DWORD StateFlags, DWORD Options);
/**
 * Sends an event to the Power Manager's policy, such as a button press or
 * a request to re-evaluate the power state.
 *
 * Windows CE 4.2 and later.
 *
 * @param dwMessage A PPN_ message, such as PPN_REEVALUATESTATE,
 *        PPN_POWERCHANGE, PPN_UNATTENDEDMODE or PPN_SUSPENDKEYPRESSED.
 * @param dwData Data for the message.
 * @return Nonzero on success, zero on failure.
 */
BOOL WINAPI PowerPolicyNotify(DWORD dwMessage, DWORD dwData);

/**
 * Gets a device's current power state from the Power Manager.
 *
 * Qualify the device name with its power class GUID, for example
 * "{8DD679CE-8AB4-43c8-A14A-EA4963FAA715}\DSK1:"; without one the generic
 * power-manageable class is assumed. Normally the Power Manager returns its
 * cached state; add POWER_FORCE to dwDeviceFlags to query the driver with
 * IOCTL_POWER_GET instead. Windows CE 4.1 and later.
 *
 * @param pvDevice Device name as an LPWSTR, such as L"COM1:".
 * @param dwDeviceFlags POWER_NAME, optionally with POWER_FORCE.
 * @param pDeviceState Receives the device power state.
 * @return ERROR_SUCCESS on success, otherwise a Win32 error code.
 */
DWORD WINAPI GetDevicePower(PVOID pvDevice, DWORD dwDeviceFlags, PCEDEVICE_POWER_STATE pDeviceState);
/**
 * Tells the Power Manager that a bus or parent driver manages power for a
 * child or client device.
 *
 * The parent can register each child instance as it appears. If pCaps is
 * NULL the parent must answer IOCTL_POWER_CAPABILITIES for the child. Free
 * the returned handle with ReleasePowerRelationship. Windows CE 4.1 and later.
 *
 * @param pvParent Parent or bus device name as an LPWSTR, such as L"NDS0:".
 * @param pvChild Child device name as an LPWSTR.
 * @param pCaps The child's POWER_CAPABILITIES, or NULL.
 * @param Flags POWER_NAME.
 * @return Relationship handle. Microsoft's page lists ERROR_SUCCESS,
 *         ERROR_INVALID_PARAMETER and ERROR_FILE_EXISTS as results despite the
 *         HANDLE return type; treat NULL as failure.
 */
HANDLE WINAPI RegisterPowerRelationship(PVOID pvParent, PVOID pvChild, PPOWER_CAPABILITIES pCaps, DWORD Flags);
/**
 * Forces a device into a given power state through the Power Manager.
 *
 * Qualify the device name with its power class GUID, as for GetDevicePower.
 * Applications should use SetPowerRequirement instead, since this overrides
 * the device's own power management; pass PwrDeviceUnspecified to hand
 * control back. Trusted callers only. Windows CE 4.1 and later.
 *
 * @param pvDevice Device name as an LPWSTR, such as L"COM1:".
 * @param dwDeviceFlags POWER_NAME.
 * @param DeviceState Power state to set, D0 to D4, or PwrDeviceUnspecified.
 * @return ERROR_SUCCESS, or ERROR_FILE_NOT_FOUND, ERROR_WRITE_FAULT,
 *         ERROR_INVALID_PARAMETER or ERROR_ACCESS_DENIED.
 */
DWORD WINAPI SetDevicePower(PVOID pvDevice, DWORD dwDeviceFlags, CEDEVICE_POWER_STATE DeviceState);

#ifdef __cplusplus
}
#endif

#endif
