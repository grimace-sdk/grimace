#ifndef GRIMACE_PKFUNCS_H
#define GRIMACE_PKFUNCS_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Passes an IOCTL to the OEM adaptation layer (OAL) in the kernel.
 *
 * The kernel doesn't process the call: it hands the parameters to the
 * OAL's OEMIoControl, as DeviceIoControl does for a driver. Which codes
 * work depends on the device's OAL.
 *
 * @param dwIoControlCode The IOCTL.
 * @param lpInBuf Input buffer, or NULL.
 * @param nInBufSize Size of lpInBuf in bytes.
 * @param lpOutBuf Output buffer, or NULL.
 * @param nOutBufSize Size of lpOutBuf in bytes.
 * @param lpBytesReturned Receives the number of bytes written to lpOutBuf.
 * @return TRUE on success, FALSE on failure (see GetLastError).
 *
 * @note Windows CE 2.0 and later.
 */
BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);
/**
 * Registers an event for a hardware interrupt and enables the interrupt.
 *
 * Call it before waiting on hEvent: the kernel signals hEvent when the
 * interrupt's ISR returns idInt. Wait on hEvent only with
 * WaitForSingleObject; WaitForMultipleObjects fails with it, and waiting
 * before this call makes this call fail. The kernel passes pvData to the
 * OAL's OEMInterruptEnable.
 *
 * @param idInt SYSINTR identifier of the interrupt.
 * @param hEvent Event, from CreateEvent, for the interrupt service thread
 *        to wait on.
 * @param pvData Data for OEMInterruptEnable, or NULL.
 * @param cbData Size of pvData in bytes.
 * @return TRUE on success, FALSE on failure.
 *
 * @note Windows CE 2.0 and later.
 */
BOOL InterruptInitialize(DWORD idInt, HANDLE hEvent, LPVOID pvData, DWORD cbData);
/**
 * Tells the kernel the driver has finished with an interrupt.
 *
 * Call it after handling each interrupt, before waiting on the event
 * again: it unmasks the interrupt, through the OAL's OEMInterruptDone.
 *
 * @param idInt SYSINTR identifier of the interrupt.
 *
 * @note Windows CE 2.0 and later.
 */
VOID InterruptDone(DWORD idInt);
/**
 * Disables a hardware interrupt and unregisters its event.
 *
 * Call it before closing the event InterruptInitialize registered. The
 * kernel calls the OAL's OEMInterruptDisable.
 *
 * @param idInt SYSINTR identifier of the interrupt.
 *
 * @note Windows CE 2.0 and later.
 */
VOID InterruptDisable(DWORD idInt);
/**
 * Maps memory from one process into another's address space.
 *
 * The CE 6.0 form of VirtualCopy, across processes: drivers use it to give
 * an application a view of a buffer they allocated.
 *
 * @param hDstProc Process to map into.
 * @param lpvDest Address in hDstProc, reserved with VirtualAllocEx.
 * @param hSrcProc Process the memory belongs to.
 * @param lpvSrc Address in hSrcProc.
 * @param cbSize Bytes to map.
 * @param fdwProtect PAGE_ protection for the mapping.
 * @return TRUE on success, FALSE on failure (see GetLastError).
 */
BOOL VirtualCopyEx(HANDLE hDstProc, LPVOID lpvDest, HANDLE hSrcProc, LPVOID lpvSrc, DWORD cbSize, DWORD fdwProtect);
/**
 * Allocates physically contiguous memory, for DMA or a frame buffer.
 *
 * Free it with FreePhysMem.
 *
 * @param cbSize Bytes to allocate.
 * @param fdwProtect PAGE_ protection.
 * @param dwAlignmentMask Alignment mask for the physical address, or 0.
 * @param dwFlags Reserved, 0.
 * @param pPhysicalAddress Receives the physical address.
 * @return The memory's virtual address, or NULL on failure.
 */
LPVOID AllocPhysMem(DWORD cbSize, DWORD fdwProtect, DWORD dwAlignmentMask, DWORD dwFlags, PULONG pPhysicalAddress);
/**
 * Frees memory from AllocPhysMem.
 *
 * @param lpvAddress Address AllocPhysMem returned.
 * @return TRUE on success, FALSE on failure.
 */
BOOL FreePhysMem(LPVOID lpvAddress);
/**
 * Returns the ID of the process whose address space the caller is
 * working in: in a driver, the application that called it.
 *
 * @return The process ID.
 */
DWORD GetCallerVMProcessId(VOID);

#if GRIMACE_CE >= 6
#define SH_GDI 80
#define SH_WMGR 81
#define SH_COMM 83
#define SH_FILESYS_APIS 84
#define SH_SHELL 85
#define SH_DEVMGR_APIS 86
#else
#define SH_LAST_NOTIFY 16
#define SH_GDI 16
#define SH_WMGR 17
#define SH_INIT 18
#define SH_COMM 19
#define SH_FILESYS_APIS 20
#define SH_SHELL 21
#define SH_DEVMGR_APIS 22
#define SH_TAPI 23
#define SH_PATCHER 24
#define SH_IMM 25
#define SH_WNET 26
#endif

/**
 * Waits until a system API set, such as GWES or the file system, has
 * registered.
 *
 * Use it in drivers and startup programs that may run before the API set
 * they need. It replaces polling IsAPIReady. Windows CE 6.0 and later.
 *
 * @param dwAPISlot API set index, such as SH_WMGR, SH_GDI or
 *        SH_FILESYS_APIS.
 * @param dwTimeout Milliseconds to wait, 0 or INFINITE.
 * @return WAIT_OBJECT_0 when the API set is ready, WAIT_TIMEOUT if it
 *         isn't ready in time, or WAIT_FAILED for an invalid index.
 */
DWORD WaitForAPIReady(DWORD dwAPISlot, DWORD dwTimeout);

#define CACHE_SYNC_DISCARD 0x001
#define CACHE_SYNC_INSTRUCTIONS 0x002
#define CACHE_SYNC_WRITEBACK 0x004
#define CACHE_SYNC_FLUSH_I_TLB 0x008
#define CACHE_SYNC_FLUSH_D_TLB 0x010
#define CACHE_SYNC_FLUSH_TLB (CACHE_SYNC_FLUSH_I_TLB | CACHE_SYNC_FLUSH_D_TLB)
#define CACHE_SYNC_L2_WRITEBACK 0x020
#define CACHE_SYNC_L2_DISCARD 0x040
#define CACHE_SYNC_ALL 0x07F

void CacheSync(int iFlags);
/**
 * Writes back and/or discards CPU cache lines covering a range of memory.
 *
 * Use it around DMA and when writing code to memory before running it.
 * Prefer this to CacheSync where the range is known, since it can avoid
 * flushing the whole cache. Windows CE 4.2 and later.
 *
 * @param pAddr Start of the range, or NULL with dwLength 0 for the whole
 *        cache.
 * @param dwLength Length of the range in bytes.
 * @param dwFlags CACHE_SYNC_ flags: CACHE_SYNC_WRITEBACK,
 *        CACHE_SYNC_DISCARD, CACHE_SYNC_INSTRUCTIONS, CACHE_SYNC_FLUSH_TLB
 *        and so on.
 */
void CacheRangeFlush(LPVOID pAddr, DWORD dwLength, DWORD dwFlags);

#ifdef __cplusplus
}
#endif

#endif
