#ifndef GRIMACE_EXTRAS_COREDLL_H
#define GRIMACE_EXTRAS_COREDLL_H

#ifndef RC_INVOKED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Loads and starts a service in services.exe.
 *
 * Services.exe reads the service's DLL and settings from
 * HKEY_LOCAL_MACHINE\Services\lpszDevKey and loads it as it would at boot.
 * Windows CE 4.1 and later.
 *
 * @param lpszDevKey Name of the service's key under HKEY_LOCAL_MACHINE\Services.
 * @param dwClientInfo Reserved. Set to 0.
 * @return Handle to the service, or NULL on failure. Call GetLastError for
 *         details.
 */
HANDLE WINAPI ActivateService(LPCWSTR lpszDevKey, DWORD dwClientInfo);
/**
 * Announces, or withdraws, a device interface that a driver exposes.
 *
 * Device Manager keeps track of advertised interfaces and notifies
 * listeners. Withdraw every interface you advertised before the driver
 * unloads. Windows CE 4.1 and later.
 *
 * @param devclass Device interface class GUID. Must not be NULL.
 * @param name Name of this interface instance, such as "DSK2:". Must be
 *        unique within the interface's namespace.
 * @param fAdd TRUE to advertise the interface, FALSE to withdraw it.
 * @return TRUE on success, FALSE on failure, including withdrawing an
 *         interface that was never advertised.
 */
BOOL WINAPI AdvertiseInterface(const GUID *devclass, LPCWSTR name, BOOL fAdd);
/**
 * Registers a synchronization partner with an EDB volume.
 *
 * Part of EDB change tracking; fails with ERROR_INVALID_PARAMETER on a CEDB
 * volume. Windows CE 5.1 and later.
 *
 * @param pVolGuid Mounted volume, or the shared volume from
 *        CREATE_SYSTEMGUID. Must not be NULL.
 * @param pSyncPartnerGuid GUID identifying the partner. Must not be NULL.
 * @param pwszFriendlyName Partner's friendly name, at most 128 characters.
 * @param pwszFullName Partner's fully qualified name, at most 128
 *        characters.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeAddSyncPartner(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, LPCWSTR pwszFriendlyName, LPCWSTR pwszFullName);
/**
 * Re-marshals a caller's buffer so a server can keep using it after the API
 * call returns.
 *
 * For drivers and servers handling API calls. Call it before the call
 * returns, on a buffer already marshalled by CeOpenCallerBuffer, and free it
 * with CeFreeAsynchronousBuffer. In user mode, only buffers marshalled with
 * CeOpenCallerBuffer work; a parameter the kernel marshalled automatically
 * becomes inaccessible when the call returns. Buffers from
 * CeAllocDuplicateBuffer do not need this. Windows CE 6.0 and later.
 *
 * @param ppDestAsyncMarshalled Receives a pointer usable after the call
 *        returns.
 * @param pSrcSyncMarshalled Buffer already marshalled for synchronous
 *        access.
 * @param cbSrc Size of the buffer in bytes. Can be 0 for ARG_I_WSTR and
 *        similar string descriptors; otherwise includes the terminator.
 * @param ArgumentDescriptor ARG_ descriptor for the buffer, such as
 *        ARG_I_WSTR or ARG_O_PTR. Not ARG_DW. In the kernel process,
 *        MARSHAL_FORCE_ALIAS forces an alias instead of a copy on ARM CPUs
 *        with a virtually tagged cache.
 * @return S_OK on success, or E_ACCESSDENIED, E_INVALIDARG or
 *         E_OUTOFMEMORY. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI CeAllocAsynchronousBuffer(PVOID *ppDestAsyncMarshalled, PVOID pSrcSyncMarshalled, DWORD cbSrc, DWORD ArgumentDescriptor);
/**
 * Copies a marshalled API parameter to a private heap buffer so the caller
 * cannot change it during the call.
 *
 * For drivers and servers handling API calls; use it only for API
 * parameters, and only where a secure copy is needed. Input descriptors
 * copy the data in; CeFreeDuplicateBuffer frees the copy and writes output
 * descriptors back. Do not use it on a buffer from CeOpenCallerBuffer.
 * Windows CE 6.0 and later.
 *
 * @param ppDestDuplicate Receives the new heap buffer, or NULL on failure.
 * @param pSrcMarshalled Caller's already marshalled buffer.
 * @param cbSrc Size of the buffer in bytes. Can be 0 for string
 *        descriptors; otherwise includes the terminator.
 * @param ArgumentDescriptor ARG_ descriptor for the buffer, such as
 *        ARG_I_WSTR or ARG_O_PTR. Not ARG_DW.
 * @return S_OK on success, or E_ACCESSDENIED, E_INVALIDARG or
 *         E_OUTOFMEMORY. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI CeAllocDuplicateBuffer(PVOID *ppDestDuplicate, PVOID pSrcMarshalled, DWORD cbSrc, DWORD ArgumentDescriptor);
/**
 * Attaches custom synchronization data to a changed record in an EDB change
 * enumeration.
 *
 * Fails with ERROR_INVALID_PARAMETER on a CEDB volume. Windows CE 5.1 and
 * later.
 *
 * @param hChangeEnum Change enumeration handle from CeGetChangedRecords.
 * @param oidChangeRecord Changed record, from the enumeration.
 * @param rgbData Custom tracking data. Must not be NULL.
 * @param ccb Size of rgbData in bytes, at most 1000.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeAttachCustomTrackingData(HANDLE hChangeEnum, CEOID oidChangeRecord, BYTE *rgbData, DWORD ccb);
/**
 * Calls a function in a DLL that is loaded outside the kernel.
 *
 * Lets a kernel-mode driver run code, typically UI, in user mode. From a
 * kernel-mode driver the call goes through the UI proxy driver in
 * udevice.exe (SYSGEN_UIPROXY); from user mode coredll calls the DLL in the
 * caller's process. The target runs as a PSL call, so it cannot use TLS;
 * start a thread if it needs to. Windows CE 6.0 and later.
 *
 * @param pszDllName DLL to load.
 * @param pszFuncName Exported function to call, of type PFN_UIENTRYPOINT.
 * @param lpInBuffer Data passed to the function.
 * @param nInBufferSize Size of lpInBuffer in bytes.
 * @param lpOutBuffer Receives data returned by the function.
 * @param nOutBufferSize Size of lpOutBuffer in bytes.
 * @param lpBytesReturned Receives the number of bytes written to
 *        lpOutBuffer.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeCallUserProc(LPCWSTR pszDllName, LPCWSTR pszFuncName, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned);
/**
 * Releases a buffer marshalled by CeOpenCallerBuffer.
 *
 * Writes back to the caller's buffer for ARG_IO and ARG_O descriptors. Pass
 * the same values given to CeOpenCallerBuffer. Resources are freed even if
 * the write-back fails. Windows CE 6.0 and later.
 *
 * @param pDestMarshalled Pointer returned by CeOpenCallerBuffer.
 * @param pSrcUnmarshalled Source pointer passed to CeOpenCallerBuffer.
 * @param cbSrc Size passed to CeOpenCallerBuffer.
 * @param ArgumentDescriptor Descriptor passed to CeOpenCallerBuffer.
 * @return S_OK on success, E_ACCESSDENIED if the write-back failed, or
 *         E_INVALIDARG. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI CeCloseCallerBuffer(PVOID pDestMarshalled, PVOID pSrcUnmarshalled, DWORD cbSrc, DWORD ArgumentDescriptor);
/**
 * Creates an EDB session for grouping changes into transactions.
 *
 * Open databases in the session with CeOpenDatabaseInSession, then use
 * CeBeginTransaction and CeEndTransaction. A transaction cannot span
 * volumes. Close with CloseHandle; an open transaction is rolled back.
 * EDB only. Windows CE 5.1 and later.
 *
 * @param pGuid Mounted volume, from CeMountDBVolEx.
 * @return Session handle, or INVALID_HANDLE_VALUE on failure. Call
 *         GetLastError for details.
 */
HANDLE WINAPI CeCreateSession(CEGUID *pGuid);
/**
 * Ends the synchronization session started by CeBeginSyncSession.
 *
 * Fails with ERROR_INVALID_PARAMETER on a CEDB volume. Windows CE 5.1 and
 * later.
 *
 * @param hSession Session the synchronization runs in.
 * @param dwOutcome DB_SYNCSESSION_COMPLETE, DB_SYNCSESSION_FAILED or
 *        DB_SYNCSESSION_PARTIAL.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeEndSyncSession(HANDLE hSession, DWORD dwOutcome);
/**
 * Commits or rolls back the transaction on an EDB session.
 *
 * Committed changes are cached until the volume is flushed (CeFlushDBVol),
 * its flush interval passes, or it is unmounted. Some locks can outlive the
 * transaction; close the database handle to release them. EDB only.
 * Windows CE 5.1 and later.
 *
 * @param hSession Session from CeCreateSession or CeGetDatabaseSession.
 * @param fCommit TRUE to commit, FALSE to roll back.
 * @return TRUE on success, FALSE on failure, including ERROR_NOT_FOUND if
 *         the session is not in a transaction. Call GetLastError for
 *         details.
 */
BOOL WINAPI CeEndTransaction(HANDLE hSession, BOOL fCommit);
/**
 * Closes a registry change notification handle.
 *
 * Windows CE 5.0 and later.
 *
 * @param hChangeHandle Handle from CeFindFirstRegChange.
 * @return Nonzero on success, zero on failure. Call GetLastError for
 *         details.
 */
BOOL WINAPI CeFindCloseRegChange(HANDLE hChangeHandle);
/**
 * Creates a handle that is signalled when a registry key changes.
 *
 * Wait on the handle, then call CeFindNextRegChange to rearm it. Close it
 * with CeFindCloseRegChange. Windows CE 5.0 and later.
 *
 * @param hKey Open key, or a predefined root such as HKEY_LOCAL_MACHINE.
 * @param bWatchSubtree TRUE to watch subkeys as well.
 * @param dwNotifyFilter REG_NOTIFY_CHANGE_LAST_SET for value changes,
 *        REG_NOTIFY_CHANGE_NAME for added or deleted subkeys, or both.
 * @return Notification handle, or INVALID_HANDLE_VALUE on failure. Call
 *         GetLastError for details.
 */
HANDLE WINAPI CeFindFirstRegChange(HKEY hKey, BOOL bWatchSubtree, DWORD dwNotifyFilter);
/**
 * Gets the next record in an EDB change enumeration.
 *
 * The returned OID identifies a change, not an ordinary record, and cannot
 * be passed to CeOidGetInfo. Fails with ERROR_INVALID_PARAMETER on a CEDB
 * volume. Windows CE 5.1 and later.
 *
 * @param hChangeEnum Change enumeration handle from CeGetChangedRecords.
 * @return OID of the next changed record, or 0 at the end or on failure.
 *         Call GetLastError for details.
 */
CEOID WINAPI CeFindNextChangedRecord(HANDLE hChangeEnum);
/**
 * Rearms a registry change notification handle after it has been
 * signalled.
 *
 * Windows CE 5.0 and later.
 *
 * @param hChangeHandle Handle from CeFindFirstRegChange.
 * @return Nonzero on success, zero on failure. Call GetLastError for
 *         details.
 */
BOOL WINAPI CeFindNextRegChange(HANDLE hChangeHandle);
/**
 * Copies changed data back from a buffer made by CeAllocAsynchronousBuffer
 * to the caller's original buffer.
 *
 * Use it to hand results back before the asynchronous buffer is freed. Only
 * ARG_O_PTR and ARG_IO_PTR descriptors write back; anything else fails. If
 * MARSHAL_FORCE_ALIAS was passed to CeAllocAsynchronousBuffer, pass it here
 * too. Windows CE 6.0 and later.
 *
 * @param pDestAsyncMarshalled Buffer returned by CeAllocAsynchronousBuffer.
 * @param pSrcSyncMarshalled Source pointer passed to
 *        CeAllocAsynchronousBuffer.
 * @param pSrcUnmarshalled Source pointer passed to CeOpenCallerBuffer, or
 *        NULL if the buffer did not come from CeOpenCallerBuffer.
 * @param cbSrc Size passed to CeAllocAsynchronousBuffer.
 * @param ArgumentDescriptor Descriptor passed to CeAllocAsynchronousBuffer.
 * @return S_OK on success, E_FAIL if the copy could not be done, or
 *         ERROR_NOT_SUPPORTED for an unsupported descriptor. Test with
 *         SUCCEEDED or FAILED.
 */
HRESULT WINAPI CeFlushAsynchronousBuffer(PVOID pDestAsyncMarshalled, PVOID pSrcSyncMarshalled, PVOID pSrcUnmarshalled, DWORD cbSrc, DWORD ArgumentDescriptor);
/**
 * Releases a buffer made by CeAllocAsynchronousBuffer, writing output data
 * back to the source first.
 *
 * Write-back happens for ARG_O_* and ARG_IO_* descriptors. Resources are
 * released even if the write-back fails. If MARSHAL_FORCE_ALIAS was passed
 * to CeAllocAsynchronousBuffer, pass it here too. Windows CE 6.0 and later.
 *
 * @param pDestAsyncMarshalled Buffer returned by CeAllocAsynchronousBuffer.
 * @param pSrcSyncMarshalled Source pointer passed to
 *        CeAllocAsynchronousBuffer.
 * @param cbSrc Size passed to CeAllocAsynchronousBuffer.
 * @param ArgumentDescriptor Descriptor passed to CeAllocAsynchronousBuffer.
 * @return S_OK on success, or E_FAIL if the write-back failed. Test with
 *         SUCCEEDED or FAILED.
 */
HRESULT WINAPI CeFreeAsynchronousBuffer(PVOID pDestAsyncMarshalled, PVOID pSrcSyncMarshalled, DWORD cbSrc, DWORD ArgumentDescriptor);
/**
 * Releases a buffer made by CeAllocDuplicateBuffer, writing output data back
 * to the source first.
 *
 * Write-back happens for ARG_O_* and ARG_IO_* descriptors. Resources are
 * released even if the write-back fails. Windows CE 6.0 and later.
 *
 * @param pDestDuplicate Buffer returned by CeAllocDuplicateBuffer.
 * @param pSrcMarshalled Source pointer passed to CeAllocDuplicateBuffer.
 * @param cbSrc Size passed to CeAllocDuplicateBuffer.
 * @param ArgumentDescriptor Descriptor passed to CeAllocDuplicateBuffer.
 * @return S_OK on success, or E_FAIL if the write-back failed. Test with
 *         SUCCEEDED or FAILED.
 */
HRESULT WINAPI CeFreeDuplicateBuffer(PVOID pDestDuplicate, PVOID pSrcMarshalled, DWORD cbSrc, DWORD ArgumentDescriptor);
/**
 * Sends an I/O control code to the file system driver behind a mount point.
 *
 * Support depends on the driver; many FSDs implement only some codes or
 * none. Windows CE 5.0 and later.
 *
 * @param pszDir Mount point of the file system, or NULL for the object
 *        store.
 * @param dwIoControlCode File system I/O control code.
 * @param lpInBuffer Input data, or NULL if the code takes none.
 * @param nInBufferSize Size of lpInBuffer in bytes.
 * @param lpOutBuffer Receives output data, or NULL if the code returns none.
 * @param nOutBufferSize Size of lpOutBuffer in bytes.
 * @param lpBytesReturned Receives the number of bytes written to
 *        lpOutBuffer.
 * @param lpOverlapped Ignored. Set to NULL.
 * @return TRUE on success, FALSE on failure. GetLastError returns
 *         ERROR_NOT_SUPPORTED if the driver does not support the call.
 */
BOOL WINAPI CeFsIoControlW(LPCWSTR pszDir, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
/**
 * Fills a buffer with random bytes.
 *
 * Anything already in the buffer is mixed in as extra seed material.
 * Windows CE 4.1 and later.
 *
 * @param dwLen Number of random bytes to generate.
 * @param pbBuffer Buffer of at least dwLen bytes that receives the data.
 * @return Nonzero on success, zero on failure.
 *
 * @note Windows CE 3.0 and later.
 */
BOOL WINAPI CeGenRandom(DWORD dwLen, BYTE *pbBuffer);
/**
 * Converts a path to its canonical form.
 *
 * Pass NULL for lpCanonicalPathName to just get the length. Windows CE 5.0
 * and later.
 *
 * @param lpPathName Path to convert, at most MAX_PATH characters. Must not be
 *        NULL.
 * @param lpCanonicalPathName Receives the canonical path. Can be NULL.
 * @param cchCanonicalPathName Size of lpCanonicalPathName in characters.
 * @param dwReserved Reserved. Set to 0.
 * @return Length of the canonical path in characters, or zero on failure.
 *         GetLastError returns ERROR_INSUFFICIENT_BUFFER if the buffer is
 *         too small.
 */
DWORD WINAPI CeGetCanonicalPathNameW(LPCWSTR lpPathName, LPWSTR lpCanonicalPathName, DWORD cchCanonicalPathName, DWORD dwReserved);
/**
 * Counts the records inserted, deleted or updated in a tracked EDB
 * database.
 *
 * EDB only; fails with ERROR_INVALID_PARAMETER on a CEDB volume. Windows CE
 * 5.1 and later.
 *
 * @param hSession Session handle from CeCreateSession.
 * @param oidDB Database to check.
 * @param pdwCnt Receives the number of changed records.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeGetChangedRecordCnt(HANDLE hSession, CEOID oidDB, DWORD *pdwCnt);
/**
 * Opens a view of the records in an EDB database that changed in the
 * current sync session's generation range.
 *
 * Call CeBeginSyncSession on the session first. Walk the result with
 * CeFindNextChangedRecord and close it with CloseHandle. EDB only. Windows
 * CE 5.1 and later.
 *
 * @param hSession Session handle with a sync session begun.
 * @param oidDB Database to enumerate.
 * @param dwChangeType DB_TRACK_INSERT, DB_TRACK_UPDATE or DB_TRACK_DELETE.
 * @return Handle to the change enumeration, or NULL on failure. Call
 *         GetLastError for details.
 */
HANDLE WINAPI CeGetChangedRecords(HANDLE hSession, CEOID oidDB, DWORD dwChangeType);
/**
 * Reads the custom sync data attached to a changed EDB record.
 *
 * The data is set with CeAttachCustomTrackingData. Pass NULL for rgbData to
 * get the size first. EDB only. Windows CE 5.1 and later.
 *
 * @param hChangeEnum Change enumeration handle from CeGetChangedRecords.
 * @param oidChangeRecord Change record OID from CeFindNextChangedRecord.
 * @param rgbData Receives the data. Can be NULL.
 * @param pccb On input, the size of rgbData in bytes. On output, the size
 *        of the data.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeGetCustomTrackingData(HANDLE hChangeEnum, CEOID oidChangeRecord, BYTE *rgbData, DWORD *pccb);
/**
 * Gets the session that an open EDB database belongs to.
 *
 * Lets you open another database in the same session and run a transaction
 * across both. Close the returned handle with CloseHandle. EDB only.
 * Windows CE 5.1 and later.
 *
 * @param hDatabase Database handle from CeOpenDatabaseInSession.
 * @return Session handle, or INVALID_HANDLE_VALUE on failure. Call
 *         GetLastError for details.
 */
HANDLE WINAPI CeGetDatabaseSession(HANDLE hDatabase);
/**
 * Reads the details of pending file change notifications.
 *
 * The change handle must come from FindFirstChangeNotification with
 * FILE_NOTIFY_CHANGE_CEGETINFO in the filter, otherwise no data is returned.
 * The buffer is filled with FILE_NOTIFY_INFORMATION records. Pass a NULL
 * buffer and zero length to just get the number of bytes available. On
 * success GetLastError returns ERROR_MORE_DATA if more is waiting, and
 * ERROR_INSUFFICIENT_BUFFER if a notification did not fit. Windows CE 4.2
 * and later.
 *
 * @param h Handle from FindFirstChangeNotification.
 * @param dwFlags Reserved. Set to 0.
 * @param lpBuffer Receives the notification records.
 * @param nBufferLength Size of lpBuffer in bytes.
 * @param lpBytesReturned Receives the number of bytes written.
 * @param lpBytesAvailable Receives the number of bytes still available.
 * @return Nonzero on success, zero on failure. Call GetLastError for
 *         details.
 */
BOOL WINAPI CeGetFileNotificationInfo(HANDLE h, DWORD dwFlags, LPVOID lpBuffer, DWORD nBufferLength, LPDWORD lpBytesReturned, LPDWORD lpBytesAvailable);
/**
 * Checks whether one tracked property of a changed EDB record has changed
 * since the last sync.
 *
 * The property must be tracked with CeTrackProperty. EDB only. Windows CE
 * 5.1 and later.
 *
 * @param hChangeEnum Change enumeration handle from CeGetChangedRecords.
 * @param oidChangeRecord Change record OID from CeFindNextChangedRecord.
 * @param propid Property to check.
 * @param pfPropChanged Receives TRUE if the property changed.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeGetPropChangeInfo(HANDLE hChangeEnum, CEOID oidChangeRecord, CEPROPID propid, BOOL *pfPropChanged);
/**
 * Reads a millisecond clock that only ever counts up.
 *
 * The value is the real-time clock plus an offset adjusted on every time
 * change, so setting the clock does not make it jump. Its absolute value
 * means nothing; use it for intervals. It is only monotonic within one
 * boot; to measure across reboots, save CeGetRawTimeOffset on each time
 * change notification and allow for it. Windows CE 6.0 and later.
 *
 * @param pui64Millsec Receives the time in milliseconds.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeGetRawTime(ULONGLONG *pui64Millsec);
/**
 * Reads the offset CeGetRawTime currently adds to the real-time clock.
 *
 * Windows CE 6.0 and later.
 *
 * @param pi64Offset Receives the offset in milliseconds.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeGetRawTimeOffset(LONGLONG *pi64Offset);
/**
 * Makes the calling thread take on the access rights of the current
 * process.
 *
 * Needs the impersonate privilege. Microsoft documents little more than
 * that. Windows CE 6.0 and later.
 *
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI CeImpersonateCurrentProcess(void);
/**
 * Gets the CeLog zones that are enabled and the zones the kernel supports.
 *
 * Any pointer can be NULL if that value is not wanted. Windows CE 4.1 and
 * later.
 *
 * @param lpdwZoneUser Receives the enabled user-defined zones.
 * @param lpdwZoneCE Receives the enabled system zones.
 * @param lpdwZoneProcess Receives the process zone mask.
 * @param lpdwAvailableZones Receives the zones the kernel supports.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI CeLogGetZones(LPDWORD lpdwZoneUser, LPDWORD lpdwZoneCE, LPDWORD lpdwZoneProcess, LPDWORD lpdwAvailableZones);
/**
 * Makes CeLog log every current process and thread again.
 *
 * For a viewer that has stopped and restarted logging and needs to know
 * what is running. Other programs can stall while the data is collected.
 * Windows CE 4.1 and later.
 *
 * @return TRUE if the resync happened, FALSE on failure.
 *
 * @note Windows CE 3.0 and later.
 */
BOOL WINAPI CeLogReSync(void);
/**
 * Marks a changed EDB record, and all its tracked properties, as changed or
 * unchanged.
 *
 * A record marked changed shows up again in the next sync session. Marking
 * it unchanged only affects the sync partner of the current session. EDB
 * only. Windows CE 5.1 and later.
 *
 * @param hChangeEnum Change enumeration handle from CeGetChangedRecords.
 * @param oidChangeRecord Change record OID from CeFindNextChangedRecord.
 * @param fChanged TRUE to mark the record changed, FALSE to mark it
 *        unchanged.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeMarkRecord(HANDLE hChangeEnum, CEOID oidChangeRecord, BOOL fChanged);
/**
 * Checks access to a caller's buffer and makes it usable from the current
 * process for the length of the call.
 *
 * For servers and drivers handling a call from another process. The
 * result may be an alias or a heap copy; release it with
 * CeCloseCallerBuffer, which also writes back output data. To keep using
 * the buffer after the call returns, wrap it with
 * CeAllocAsynchronousBuffer. Nothing is allocated if the call fails or the
 * source is NULL. Windows CE 6.0 and later.
 *
 * @param ppDestMarshalled Receives the pointer to use.
 * @param pSrcUnmarshalled Caller's buffer.
 * @param cbSrc Size of the caller's buffer in bytes, including the
 *        terminator for strings. Can be 0 for ARG_I_WSTR and ARG_I_ASTR
 *        style descriptors.
 * @param ArgumentDescriptor Kind of argument, such as ARG_I_WSTR or
 *        ARG_O_PTR. ARG_DW is not valid.
 * @param ForceDuplicate TRUE to always copy to a private heap buffer, so
 *        the caller can't change it mid-call. FALSE lets the system pick
 *        the fastest method.
 * @return S_OK on success, or E_ACCESSDENIED, E_INVALIDARG or
 *         E_OUTOFMEMORY. Test with SUCCEEDED or FAILED.
 */
HRESULT WINAPI CeOpenCallerBuffer(PVOID *ppDestMarshalled, PVOID pSrcUnmarshalled, DWORD cbSrc, DWORD ArgumentDescriptor, BOOL ForceDuplicate);
/**
 * Opens a stream on a CEVT_STREAM property of the current EDB record.
 *
 * Lets large values be read and written in pieces with CeStreamRead,
 * CeStreamWrite, CeStreamSeek and CeStreamSetSize. Only one stream per
 * database handle can be open for writing, and writes must be committed
 * with CeStreamSaveChanges. Moving the database's seek position abandons
 * the stream. Compressed properties can't be opened as streams. Close with
 * CloseHandle. EDB only. Windows CE 5.1 and later.
 *
 * @param hDatabase Database handle positioned on the record.
 * @param propid CEVT_STREAM property to open.
 * @param dwMode GENERIC_READ, or GENERIC_WRITE for read and write access.
 * @return Stream handle, or INVALID_HANDLE_VALUE on failure. Call
 *         GetLastError for details.
 */
HANDLE WINAPI CeOpenStream(HANDLE hDatabase, CEPROPID propid, DWORD dwMode);
/**
 * Discards change tracking data older than a given time for one sync
 * partner.
 *
 * Use it periodically to save space. EDB only. Windows CE 5.1 and later.
 *
 * @param pVolGuid Mounted volume, or the shared system volume.
 * @param pSyncPartnerGuid Sync partner.
 * @param pstThreshold Change information gathered before this time is
 *        purged.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CePurgeTrackingData(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, SYSTEMTIME *pstThreshold);
/**
 * Reads the details of pending registry change notifications.
 *
 * The buffer is filled with REG_NOTIFY_INFORMATION records. Pass a NULL
 * buffer and zero length to just get the number of bytes available. On
 * success GetLastError returns ERROR_MORE_DATA if more is waiting, and
 * ERROR_INSUFFICIENT_BUFFER if a notification did not fit. Windows CE 5.1
 * and later.
 *
 * @param hChangeHandle Handle from CeFindFirstRegChange.
 * @param dwFlags Unused. Set to 0.
 * @param lpBuffer Receives the notification records. Can be NULL.
 * @param nBufferLength Size of lpBuffer in bytes.
 * @param lpBytesReturned Receives the number of bytes written. Can be NULL.
 * @param lpBytesAvailable Receives the number of bytes available.
 * @return Nonzero on success, zero on failure. Call GetLastError for
 *         details.
 */
LONG WINAPI CeRegGetNotificationInfo(HANDLE hChangeHandle, DWORD dwFlags, LPVOID lpBuffer, DWORD nBufferLength, LPDWORD lpBytesReturned, LPDWORD lpBytesAvailable);
/**
 * Sets a registry value only if its current contents match, as one atomic
 * operation.
 *
 * A match needs the same type, size and data. Use it to update shared
 * settings without a race. Windows CE 5.1 and later.
 *
 * @param hKey Open key or a predefined root such as HKEY_LOCAL_MACHINE.
 * @param lpValueName Value name, or NULL or "" for the key's unnamed value.
 * @param dwType Value type, such as REG_DWORD or REG_SZ.
 * @param lpOldData Contents to compare with. Can be NULL.
 * @param cbOldData Size of lpOldData in bytes.
 * @param lpNewData Contents to write on a match. Can be NULL.
 * @param cbNewData Size of lpNewData in bytes.
 * @param dwFlags 0 to set only if the value exists and matches,
 *        REG_FLAGS_TESTSET_NEW to also create a missing value, or
 *        REG_FLAGS_TESTSET_NOMATCH to set only if it does not match.
 * @return ERROR_SUCCESS if the value was set, ERROR_FILE_NOT_FOUND if it
 *         does not exist, ERROR_NO_MATCH if it did not match, or another
 *         Win32 error code.
 */
LONG WINAPI CeRegTestSetValueW(HKEY hKey, LPCWSTR lpValueName, DWORD dwType, const BYTE *lpOldData, DWORD cbOldData, const BYTE *lpNewData, DWORD cbNewData, DWORD dwFlags);
/**
 * Creates a heap that a server can allocate from on behalf of a client
 * process.
 *
 * The client gets read-only access unless HEAP_CLIENT_READWRITE is given.
 * Allocate with HeapAlloc and hand pointers to the client through
 * CeRemoteHeapTranslatePointer. Windows CE 6.0 and later.
 *
 * @param dwProcessId Client process the heap is shared with.
 * @param dwFlags 0 or HEAP_CLIENT_READWRITE.
 * @return Heap handle, or NULL on failure.
 */
HANDLE WINAPI CeRemoteHeapCreate(DWORD dwProcessId, DWORD dwFlags);
/**
 * Converts a pointer into a remote heap between the server's and the
 * client's address.
 *
 * Windows CE 6.0 and later.
 *
 * @param hHeap Heap from CeRemoteHeapCreate.
 * @param dwFlags 0 to translate a server pointer for the client, or
 *        CE_HEAP_REVERSE_TRANSLATE for the other way round.
 * @param ptr Pointer from HeapAlloc or HeapReAlloc on the heap, or a client
 *        pointer with CE_HEAP_REVERSE_TRANSLATE.
 * @return The translated pointer.
 */
LPVOID WINAPI CeRemoteHeapTranslatePointer(HANDLE hHeap, DWORD dwFlags, LPVOID ptr);
/**
 * Removes property definitions from an EDB database.
 *
 * The database must not be open, and the properties must not be part of a
 * sort order. Can shrink the volume. EDB only. Windows CE 5.1 and later.
 *
 * @param pGuid Volume holding the database.
 * @param oidDb Database to change.
 * @param cProps Number of entries in prgProps.
 * @param prgProps Property IDs to remove.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeRemoveDatabaseProps(PCEGUID pGuid, CEOID oidDb, DWORD cProps, CEPROPID *prgProps);
/**
 * Stops change tracking on an EDB database for one sync partner.
 *
 * The data is untouched; the change history for that partner is purged.
 * Use CREATE_SYSTEMGUID for the shared system volume. EDB only. Windows CE
 * 5.1 and later.
 *
 * @param pVolGuid Mounted volume, or the shared system volume.
 * @param pSyncPartnerGuid Sync partner.
 * @param oid Tracked database.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeRemoveDatabaseTracking(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, CEOID oid);
/**
 * Removes a sync partner from a mounted EDB volume.
 *
 * Use CREATE_SYSTEMGUID for the shared system volume. EDB only. Windows CE
 * 5.1 and later.
 *
 * @param pVolGuid Mounted volume, or the shared system volume.
 * @param pSyncPartnerGuid Sync partner to remove.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeRemoveSyncPartner(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid);
/**
 * Moves the seek position of an open database and returns the record it
 * lands on.
 *
 * Value seeks use the sort order the database was opened with, and
 * dwValue then points to an array of CEPROPVAL. For CEDB_SEEK_CURRENT,
 * cast a negative count to DWORD to seek backwards. On failure the
 * position is unchanged for CEDB; value seeks that find nothing leave it
 * at the end. EDB adds CEDB_SEEK_PREFIX and the ...OREQUAL seek types, and
 * in EDB CEDB_SEEK_VALUEGREATER and CEDB_SEEK_VALUESMALLER exclude equal
 * values, unlike CEDB. Windows CE 4.1 and later.
 *
 * @param hDatabase Open database handle.
 * @param dwSeekType A CEDB_SEEK_* value.
 * @param dwValue Record count, OID, or CEPROPVAL array pointer, depending
 *        on dwSeekType.
 * @param wNumVals Number of CEPROPVAL entries, for value seeks.
 * @param lpdwIndex Receives the record's index. Can be NULL; on CEDB
 *        passing it makes the seek slower.
 * @return OID of the record found, or 0 on failure. Call GetLastError for
 *         details.
 */
CEOID WINAPI CeSeekDatabaseEx(HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue, WORD wNumVals, LPDWORD lpdwIndex);
/**
 * Asks the kernel to set an event when the system resumes.
 *
 * For power handlers. The kernel only checks for the event at the end of
 * its resume path, so calling this outside a resume callback takes effect
 * at the next resume. Windows CE 4.1 and later.
 *
 * @param hEvt Event handle.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI CeSetPowerOnEvent(HANDLE hEvt);
/**
 * Sets the version recorded for a process.
 *
 * Windows CE 4.2 and later.
 *
 * @param hProcess Process handle.
 * @param dwVersion MAKELONG(minor, major).
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI CeSetProcessVersion(HANDLE hProcess, DWORD dwVersion);
/**
 * Sets an option on an EDB session.
 *
 * EDB only. Windows CE 5.1 and later.
 *
 * @param hSession Session from CeCreateSession or CeGetDatabaseSession.
 * @param ulOptionId DBOPT_SESSION_LOCK_TIMEOUT, the lock wait in
 *        milliseconds before a transaction times out, or
 *        DBOPT_SESSION_MAX_CHANGES, the number of change notifications
 *        queued in a transaction (0 sends only DB_NOTIFY_VOLUME_CHANGED).
 * @param dwValue New value for the option.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeSetSessionOption(HANDLE hSession, ULONG ulOptionId, DWORD dwValue);
/**
 * Reads bytes from an EDB stream at its current position.
 *
 * Reading less than cbRead at the end is not an error. The position
 * advances by the bytes read. A stream opened for reading does not lock
 * the record, so it can change or be deleted underneath; read inside a
 * repeatable read transaction to avoid that. EDB only. Windows CE 5.1 and
 * later.
 *
 * @param hStream Stream from CeOpenStream with GENERIC_READ.
 * @param lprgbBuffer Receives the data.
 * @param cbRead Number of bytes to read.
 * @param lpcbRead Receives the number of bytes read. Can be NULL.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeStreamRead(HANDLE hStream, LPBYTE lprgbBuffer, DWORD cbRead, LPDWORD lpcbRead);
/**
 * Commits data written to an EDB stream.
 *
 * Afterwards the stream is read-only; reopen it to write more. EDB only.
 * Windows CE 5.1 and later.
 *
 * @param hStream Stream from CeOpenStream with GENERIC_WRITE.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeStreamSaveChanges(HANDLE hStream);
/**
 * Moves the read/write position of an EDB stream.
 *
 * No data is read, so seeking past the end succeeds and the next read or
 * write fails. Seek 0 from STREAM_SEEK_END to get the stream's size. EDB
 * only. Windows CE 5.1 and later.
 *
 * @param hStream Stream from CeOpenStream.
 * @param cbMove Offset in bytes. For STREAM_SEEK_CUR, cast a negative LONG
 *        to move backwards.
 * @param dwOrigin STREAM_SEEK_SET, STREAM_SEEK_CUR or STREAM_SEEK_END.
 * @param lpcbNewOffset Receives the new position from the start. Can be
 *        NULL.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeStreamSeek(HANDLE hStream, DWORD cbMove, DWORD dwOrigin, LPDWORD lpcbNewOffset);
/**
 * Grows or truncates an EDB stream.
 *
 * Commit with CeStreamSaveChanges. Setting the size before writing a new
 * or growing stream is faster than letting writes extend it. EDB only.
 * Windows CE 5.1 and later.
 *
 * @param hStream Stream from CeOpenStream with GENERIC_WRITE.
 * @param cbSize New size in bytes.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeStreamSetSize(HANDLE hStream, DWORD cbSize);
/**
 * Writes bytes to an EDB stream at its current position.
 *
 * Overwrites existing data and extends the stream past the end, but never
 * truncates; use CeStreamSetSize for that. The position advances by the
 * bytes written. Commit with CeStreamSaveChanges. EDB only. Windows CE 5.1
 * and later.
 *
 * @param hStream Stream from CeOpenStream with GENERIC_WRITE.
 * @param lprgbBuffer Data to write.
 * @param cbWrite Number of bytes to write.
 * @param lpcbWritten Receives the number of bytes written. Can be NULL.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeStreamWrite(HANDLE hStream, LPBYTE lprgbBuffer, DWORD cbWrite, LPDWORD lpcbWritten);
/**
 * Starts change tracking on an EDB database for a sync partner.
 *
 * Up to 10 sync partners per database. Use CREATE_SYSTEMGUID for the
 * shared system volume. EDB only. Windows CE 5.1 and later.
 *
 * @param pVolGuid Mounted volume, or the shared system volume.
 * @param pSyncPartnerGuid Sync partner, added with CeAddSyncPartner.
 * @param oid Database to track.
 * @param dwTrackingFlags Change types to track (DB_TRACK_INSERT,
 *        DB_TRACK_UPDATE, DB_TRACK_DELETE, DB_TRACK_ROWIDENTITY_*) combined
 *        with one DB_TRACK_LEVEL_* mode.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeTrackDatabase(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, CEOID oid, DWORD dwTrackingFlags);
/**
 * Includes or excludes one property from change tracking on an EDB
 * database.
 *
 * EDB only. Windows CE 5.1 and later.
 *
 * @param pVolGuid Mounted volume, or the shared system volume.
 * @param oidDB Tracked database.
 * @param propid Property to include or exclude.
 * @param fTrack TRUE to track changes to the property, FALSE to ignore
 *        them.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI CeTrackProperty(PCEGUID pVolGuid, CEOID oidDB, CEPROPID propid, BOOL fTrack);
/**
 * Allocates memory that the caller can write and every other process can
 * read.
 *
 * Reserve with MEM_RESERVE and a NULL address, then commit with
 * MEM_COMMIT, or pass both to do it in one call. Free with VirtualFree.
 * Fails with ERROR_ACCESS_DENIED for callers that are not fully trusted;
 * on CE 6.0 it is kernel mode only. Windows CE 5.0 and later.
 *
 * @param lpvAddr Address to commit, previously reserved by this function,
 *        or NULL to reserve.
 * @param cbSize Size in bytes.
 * @param fdwAction MEM_RESERVE, MEM_COMMIT or both.
 * @return Address of the region, or NULL on failure. Call GetLastError for
 *         details.
 */
LPVOID WINAPI CeVirtualSharedAlloc(LPVOID lpvAddr, DWORD cbSize, DWORD fdwAction);
/**
 * Converts a pointer mapped into a process slot back to its slot 0 form.
 *
 * Only meaningful on the slot-based memory model before CE 6.0; Microsoft
 * lists it as unsupported from CE 6.0. Windows CE 4.2 and later.
 *
 * @param ptr Pointer to convert.
 * @return The unmapped pointer, or ptr unchanged if it was not mapped.
 */
LPVOID WINAPI CeZeroPointer(LPVOID ptr);
/**
 * Maps a physical address range to a fixed, uncached virtual address
 * usable from an interrupt service routine.
 *
 * ISRs can only touch statically mapped addresses; this creates one at
 * run time instead of at boot. The mapping lasts until DeleteStaticMapping
 * or a reboot. Windows CE 4.1 and later.
 *
 * @param dwPhysBase Physical address shifted right by 8.
 * @param dwSize Number of bytes to map.
 * @return Uncached virtual address of the mapping, or NULL on failure.
 */
LPVOID WINAPI CreateStaticMapping(DWORD dwPhysBase, DWORD dwSize);
/**
 * Creates a named watchdog timer for a critical process.
 *
 * The process refreshes it with RefreshWatchDogTimer. If it misses a
 * period the handle is signalled, and a monitor waiting on it has dwWait
 * milliseconds to refresh it or save state before the default action. The
 * watchdog then stops until restarted. Opening an existing name returns a
 * new handle to it and sets ERROR_ALREADY_EXISTS. Close with CloseHandle.
 * Windows CE 6.0 and later.
 *
 * @param pszWatchDogName Name of the watchdog.
 * @param dwPeriod Period in milliseconds.
 * @param dwWait Grace time in milliseconds after a missed period.
 * @param dwDfltAction WDOG_KILL_PROCESS, WDOG_RESET_DEVICE or
 *        WDOG_NO_DFLT_ACTION.
 * @param dwParam Passed to IOCTL_HAL_REBOOT on reset.
 * @param dwFlags Reserved. Set to 0.
 * @return Watchdog handle, or NULL on failure.
 */
HANDLE WINAPI CreateWatchDogTimer(LPCWSTR pszWatchDogName, DWORD dwPeriod, DWORD dwWait, DWORD dwDfltAction, DWORD dwParam, DWORD dwFlags);
/**
 * Frees a credential returned by CredRead.
 *
 * Windows CE 5.0 and later.
 *
 * @param pbBuffer Credential to free.
 * @return ERROR_SUCCESS, or a credential manager error code.
 */
DWORD WINAPI CredFree(PBYTE pbBuffer);
/**
 * Deletes a partition from a store.
 *
 * The partition must not be mounted. Windows CE 6.0 and later.
 *
 * @param hStore Store handle from OpenStore.
 * @param szPartitionName Name of the partition.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI DeletePartition(HANDLE hStore, LPCWSTR szPartitionName);
/**
 * Removes a mapping made by CreateStaticMapping.
 *
 * Windows CE 5.0 and later.
 *
 * @param pVirtBase Virtual address returned by CreateStaticMapping.
 * @param dwSize Size of the mapping in bytes.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI DeleteStaticMapping(LPVOID pVirtBase, DWORD dwSize);
/**
 * Stops and unloads a service instance in services.exe.
 *
 * The service is first sent IOCTL_SERVICE_QUERY_CAN_DEINIT and can refuse.
 * Otherwise its sockets are unbound and xxx_Deinit is called; the DLL is
 * unloaded with its last instance, so it must not leave threads running.
 * A service loaded by a privileged caller can't be stopped by a normal
 * one. Windows CE 4.1 and later.
 *
 * @param hDevice Service handle from ActivateService, RegisterService or
 *        GetServiceHandle.
 * @return Nonzero on success, zero on failure. Call GetLastError for
 *         details.
 */
BOOL WINAPI DeregisterService(HANDLE hDevice);
/**
 * Turns off touch gestures for a window or the whole process.
 *
 * ullFlags is 64-bit. Re-enable with EnableGestures. Windows CE 6.0 and
 * later.
 *
 * @param hwnd Window to change. Ignored with TGF_SCOPE_PROCESS.
 * @param ullFlags Gestures to disable: TGF_PAN, TGF_SCROLL, TGF_HOLD,
 *        TGF_SELECT, TGF_DOUBLESELECT, or GID_TO_TGF(id) for a custom
 *        gesture.
 * @param uScope TGF_SCOPE_WINDOW or TGF_SCOPE_PROCESS.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI DisableGestures(HWND hwnd, ULONGLONG ullFlags, UINT uScope);
/**
 * Dismounts a partition and its file system.
 *
 * Windows CE 6.0 and later.
 *
 * @param hPartition Partition handle from OpenPartition.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI DismountPartition(HANDLE hPartition);
/**
 * Dismounts every file system on a store.
 *
 * Windows CE 6.0 and later.
 *
 * @param hStore Store handle from OpenStore.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI DismountStore(HANDLE hStore);
/**
 * Turns on touch gestures for a window or the whole process.
 *
 * ullFlags is 64-bit. Turn them off with DisableGestures. Windows CE 6.0
 * and later.
 *
 * @param hwnd Window to change. Ignored with TGF_SCOPE_PROCESS.
 * @param ullFlags Gestures to enable: TGF_PAN, TGF_SCROLL, TGF_HOLD,
 *        TGF_SELECT, TGF_DOUBLESELECT, or GID_TO_TGF(id) for a custom
 *        gesture.
 * @param uScope TGF_SCOPE_WINDOW or TGF_SCOPE_PROCESS.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI EnableGestures(HWND hwnd, ULONGLONG ullFlags, UINT uScope);
/**
 * Calls a function for each entry in a window's property list.
 *
 * Stops early if the callback returns FALSE. Don't remove properties that
 * other code added. Windows CE 4.1 and later.
 *
 * @param hWnd Window whose properties to list.
 * @param lpEnumFunc Callback, called once per property.
 * @param lParam Value passed to the callback.
 * @return The callback's last return value, or -1 if the window has no
 *         properties.
 */
int WINAPI EnumPropsEx(HWND hWnd, PROPENUMPROCEXW lpEnumFunc, LPARAM lParam);
/**
 * Lists the services running in services.exe.
 *
 * The buffer starts with an array of ServiceEnumInfo structures, followed
 * by the DLL name strings they point to. Call with too small a buffer to
 * get the size needed, but the count can change before the next call.
 * Windows CE 4.1 and later.
 *
 * @param pBuffer Receives the service list.
 * @param pdwServiceEntries Receives the number of services.
 * @param pdwBufferLen On input, the size of pBuffer in bytes. On output,
 *        the size needed.
 * @return Nonzero on success, zero on failure. Call GetLastError for
 *         details.
 */
BOOL WINAPI EnumServices(PBYTE pBuffer, DWORD *pdwServiceEntries, DWORD *pdwBufferLen);
/**
 * Closes a partition search handle.
 *
 * Windows CE 6.0 and later.
 *
 * @param hSearch Handle from FindFirstPartition.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI FindClosePartition(HANDLE hSearch);
/**
 * Closes a store search handle.
 *
 * Windows CE 6.0 and later.
 *
 * @param hSearch Handle from FindFirstStore.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI FindCloseStore(HANDLE hSearch);
/**
 * Clears a partition so that it can take a new file system.
 *
 * Only sector zero of the partition is erased; no file system is written.
 * Call FormatVolume afterwards to lay down FATFS or similar. Windows CE 6.0
 * and later.
 *
 * @param hPartition Partition handle from OpenPartition.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI FormatPartition(HANDLE hPartition);
/**
 * Clears a partition and sets its partition type in the master boot
 * record.
 *
 * The partition must already exist (see CreatePartition). Only sector zero
 * is erased; no file system is written, so follow up with FormatVolume.
 * Windows CE 6.0 and later.
 *
 * @param hPartition Partition handle from OpenPartition.
 * @param bPartType Partition type byte to record. Ignored when bAuto is
 *        TRUE.
 * @param bAuto TRUE to let the partition driver pick the type for its
 *        default file system; FAT picks FAT12, FAT16 or FAT32 from the
 *        partition size.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI FormatPartitionEx(HANDLE hPartition, BYTE bPartType, BOOL bAuto);
/**
 * Wipes the partition table of a store.
 *
 * Writes a fresh master boot record to sector zero, which removes every
 * partition on the store. No file system is written. Windows CE 6.0 and
 * later.
 *
 * @param hStore Store handle from OpenStore.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI FormatStore(HANDLE hStore);
/**
 * Passes an I/O control on to another driver without changing the direct
 * caller.
 *
 * For drivers that relay IOCTLs they do not understand. It behaves like
 * DeviceIoControl, except that a kernel-mode caller stays the direct caller
 * instead of being replaced by the kernel, so the receiving driver can
 * validate parameters against the original caller. No validation is done
 * here. Windows CE 6.0 and later.
 *
 * @param hDevice Device handle from CreateFile.
 * @param dwIoControlCode I/O control code to send.
 * @param lpInBuf Input buffer, or NULL if the operation takes no input.
 * @param nInBufSize Size of lpInBuf in bytes.
 * @param lpOutBuf Output buffer, or NULL if the operation returns no data.
 * @param nOutBufSize Size of lpOutBuf in bytes.
 * @param lpBytesReturned Receives the number of bytes written to lpOutBuf.
 * @param lpOverlapped Ignored; pass NULL.
 * @return Nonzero on success, zero on failure; call GetLastError.
 */
BOOL WINAPI ForwardDeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
/**
 * Uninstalls an interrupt handler installed with LoadIntChainHandler.
 *
 * The handler's code stays in kernel memory until the device is reset, and a
 * later LoadIntChainHandler for the same handler reuses it. Trusted callers
 * only. Windows CE 4.1 and later.
 *
 * @param hInstance Handle returned by LoadIntChainHandler.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI FreeIntChainHandler(HANDLE hInstance);
/**
 * Gets the ABC widths of a range or list of glyph indices in the selected
 * TrueType font.
 *
 * Fails for non-TrueType fonts. A negative A or C width means the glyph
 * overhangs. Windows CE 5.0 and later.
 *
 * @param hdc Device context.
 * @param giFirst First glyph index of a consecutive range. Used only when pgi
 *        is NULL.
 * @param cgi Number of glyph indices.
 * @param pgi Array of cgi glyph indices, or NULL to use the range starting at
 *        giFirst.
 * @param lpabc Array of cgi ABC structures that receives the widths in
 *        logical units.
 * @return Nonzero on success, zero on failure; call GetLastError.
 */
BOOL WINAPI GetCharABCWidthsI(HDC hdc, UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc);
/**
 * Gets a hash of the device's hardware identifier combined with
 * application-supplied data.
 *
 * The same application data always gives the same result on the same device,
 * even after a reflash or cold boot, while different applications get
 * unrelated values, so servers cannot correlate devices across applications.
 * Callable at any trust level. Passing NULL for pbDeviceIDOutput returns the
 * required size in *pcbDeviceIDOutput. Not present on all CE 5.x ROMs.
 * Windows CE 5.1 and later.
 *
 * @param pbApplicationData Application data to hash with the device ID. At
 *        least 8 bytes, and should be unique to the application.
 * @param cbApplicationData Size of pbApplicationData in bytes, at least 8.
 * @param dwDeviceIDVersion Algorithm version; must be 1.
 * @param pbDeviceIDOutput Buffer that receives the hash
 *        (GETDEVICEUNIQUEID_V1_OUTPUT bytes), or NULL to query the size. A
 *        short buffer gets a truncated hash.
 * @param pcbDeviceIDOutput On input the size of pbDeviceIDOutput; on output
 *        the bytes written. Must not be NULL.
 * @return S_OK on success, HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER) if
 *         the hash was truncated, E_POINTER or E_INVALIDARG for bad
 *         parameters.
 */
HRESULT WINAPI GetDeviceUniqueID(LPBYTE pbApplicationData, DWORD cbApplicationData, DWORD dwDeviceIDVersion, LPBYTE pbDeviceIDOutput, DWORD *pcbDeviceIDOutput);
/**
 * Gets the process ID of the immediate caller of a driver or server.
 *
 * In kernel mode this is the kernel's process ID when a kernel component made
 * the call, or the application's ID when it called in directly. In user mode
 * it is the same as GetCallerVMProcessId. Replaces GetCallerProcess. Windows
 * CE 6.0 and later.
 *
 * @return The direct caller's process ID.
 */
DWORD WINAPI GetDirectCallerProcessId(void);
/**
 * Gets the DWORD stored with an event by SetEventData.
 *
 * Windows CE 4.1 and later.
 *
 * @param hEvent Event handle from CreateEvent.
 * @return The event's data. Zero is also returned on failure, so call
 *         GetLastError to tell them apart.
 */
DWORD WINAPI GetEventData(HANDLE hEvent);
/**
 * Gets the keyboard layout of the foreground thread.
 *
 * Intended for OEM shells, to keep the input panel or IME indicator in step
 * with the foreground application. Applications should use
 * GetKeyboardLayout. Windows CE 4.2 and later.
 *
 * @return The HKL. The low word is the input language identifier and the
 *         high word identifies the physical keyboard layout.
 */
HKL WINAPI GetForegroundKeyboardLayoutHandle(void);
/**
 * Gets the tick count at which a window's thread last became ready to process
 * a message.
 *
 * If the thread is idle in GetMessage the result equals GetTickCount, so
 * subtracting it from GetTickCount gives roughly how long the thread has been
 * busy. Useful for detecting a hung UI thread. Windows CE 4.1 and later.
 *
 * @param hWnd A window owned by the thread to query.
 * @return Milliseconds since boot when the thread was last ready, or zero on
 *         failure; call GetLastError.
 */
DWORD WINAPI GetMessageQueueReadyTimeStamp(HWND hWnd);
/**
 * Gets the bounds, work area and flags of a display.
 *
 * Pass a MONITORINFO, or a MONITORINFOEX to also get the display name. The
 * cbSize member tells the function which one it was given. Windows CE 4.1 and
 * later.
 *
 * @param hMonitor Display handle.
 * @param lpmi MONITORINFO or MONITORINFOEX with cbSize set to the size of the
 *        structure.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI GetMonitorInfo(HMONITOR hMonitor, LPMONITORINFO lpmi);
/**
 * Gets the system password flags.
 *
 * The result is a combination of PASSWORD_STATUS_ACTIVE, set when the system
 * password is enabled, and PASSWORD_STATUS_SCREENSAVERPROTECT, set when the
 * password is also required after the screen saver. An OEM-level API (built
 * with WINCEOEM=1). Windows CE 4.1 and later.
 *
 * @return The PASSWORD_STATUS_ flags currently set.
 */
DWORD WINAPI GetPasswordStatus(void);
/**
 * Gets a handle to a Services.exe service instance from its prefix.
 *
 * Deprecated: open the service with CreateFile on its prefix instead, except
 * for a service that was unloaded with DeregisterService. From CE 6.0 the
 * handle must be closed with CloseHandle and the DLL name is no longer
 * returned. Windows CE 4.1 and later.
 *
 * @param szPrefix Service prefix and index in the form "XXXN:", such as
 *        L"HTP0:".
 * @param szDllName Optional buffer that receives the service DLL name.
 * @param pdwDllBuf Optional. On input the size of szDllName in bytes; on
 *        output the bytes written or required, including the terminating
 *        null.
 * @return Service handle, or INVALID_HANDLE_VALUE on failure; call
 *         GetLastError.
 */
HANDLE WINAPI GetServiceHandle(LPWSTR szPrefix, LPWSTR szDllName, LPDWORD pdwDllBuf);
/**
 * Captures the call stack of any thread.
 *
 * Without flags each frame is a CallSnapshot holding a return address. With
 * STACKSNAP_EXTENDED_INFO each frame is a CallSnapshotEx, adding the frame
 * pointer, process and first four parameters; on CE 6.0 STACKSNAP_NEW_VM
 * selects CallSnapshot3. Other flags: STACKSNAP_FAIL_IF_INCOMPLETE,
 * STACKSNAP_INPROC_ONLY and STACKSNAP_RETURN_FRAMES_ON_ERROR. Untrusted callers
 * can only walk threads in their own process. The caller may be boosted above
 * hThrd's priority while it runs. To get a deep stack, call repeatedly,
 * adding the frames returned to dwSkip each time. Windows CE 5.0 and later.
 *
 * @param hThrd Thread whose stack to capture.
 * @param dwMaxFrames Number of entries in lpFrames.
 * @param lpFrames Array of CallSnapshot or CallSnapshotEx structures that
 *        receives the frames.
 * @param dwFlags Zero or more STACKSNAP_ flags.
 * @param dwSkip Number of innermost frames to skip.
 * @return Number of frames copied, or zero on failure; call GetLastError.
 */
ULONG WINAPI GetThreadCallStack(HANDLE hThrd, ULONG dwMaxFrames, LPVOID lpFrames[], DWORD dwFlags, DWORD dwSkip);
/**
 * Gets the full path of the current user's profile directory.
 *
 * Windows CE 4.1 and later.
 *
 * @param lpszBuffer Buffer that receives the path.
 * @param lpdwSize Size of lpszBuffer.
 * @return TRUE on success, FALSE on failure. Fails with ERROR_PATH_NOT_FOUND
 *         when no user is logged in.
 */
BOOL WINAPI GetUserDirectory(LPWSTR lpszBuffer, LPDWORD lpdwSize);
/**
 * Masks or unmasks a hardware interrupt.
 *
 * Lets a driver touch its hardware without being interrupted by its own
 * interrupt. Windows CE 4.2 and later.
 *
 * @param idInt System interrupt ID associated with the driver's IST.
 * @param fDisable TRUE to mask the interrupt, FALSE to unmask it.
 */
VOID WINAPI InterruptMask(DWORD idInt, BOOL fDisable);
/**
 * Tests whether a named event is signaled, without needing a handle to it.
 *
 * Windows CE 5.0 and later.
 *
 * @param pszName Name of the event.
 * @param dwFlags Reserved; must be zero.
 * @return Nonzero if the event is signaled, FALSE if not.
 */
BOOL WINAPI IsNamedEventSignaled(LPCWSTR pszName, DWORD dwFlags);
/**
 * Sends an I/O control to an installable ISR's IOControl function.
 *
 * Used by a driver to configure the handler it installed with
 * LoadIntChainHandler, for example passing GIISR_INFO to giisr.dll. Trusted
 * callers only. Windows CE 4.1 and later.
 *
 * @param hModule Handle returned by LoadIntChainHandler.
 * @param dwIoControlCode IOCTL defined by the ISR.
 * @param lpInBuf Input buffer, or NULL if the operation takes no input.
 * @param nInBufSize Size of lpInBuf in bytes.
 * @param lpOutBuf Output buffer, or NULL if the operation returns no data.
 * @param nOutBufSize Size of lpOutBuf in bytes.
 * @param lpBytesReturned Receives the number of bytes written to lpOutBuf.
 *        Must not be NULL.
 * @return TRUE on success, FALSE on failure; call GetLastError.
 */
BOOL WINAPI KernelLibIoControl(HANDLE hModule, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);
/**
 * Builds an animated wait cursor from a run of bitmap resources.
 *
 * The frames are cFrames bitmaps with consecutive resource IDs starting at
 * ResourceId. Use 48 x 48 bitmaps; anything larger is clipped from the top
 * left. With hInstance NULL and ResourceId IDC_WAIT, the default system wait
 * cursor is loaded and FrameTimeInterval is ignored. Fails with
 * ERROR_CALL_NOT_IMPLEMENTED if the image has no mouse cursor support.
 * Windows CE 4.1 and later.
 *
 * @param hInstance Module that holds the bitmaps.
 * @param ResourceId MAKEINTRESOURCE ID of the first frame.
 * @param cFrames Number of frames.
 * @param FrameTimeInterval Time each frame is shown, in milliseconds.
 * @return Cursor handle to pass to SetCursor, or NULL on failure. Call
 *         GetLastError for details.
 */
HCURSOR WINAPI LoadAnimatedCursor(HINSTANCE hInstance, DWORD ResourceId, int cFrames, int FrameTimeInterval);
/**
 * Installs an interrupt service routine from a DLL into the kernel for a
 * given IRQ.
 *
 * The DLL is loaded into the kernel process. It must be self-contained: no
 * dependent DLLs, no implicit links to other DLLs and no default C runtime.
 * One DLL can hold several handlers and be used by several calls. The entry
 * point has the form DWORD ISRHandler(DWORD InstanceIndex). Configure the
 * handler afterwards with KernelLibIoControl, and remove it with
 * FreeIntChainHandler. Trusted callers only. Windows CE 4.1 and later.
 *
 * @param lpFilename DLL name, with backslashes if it includes a path;
 *        without one the LoadLibrary search order applies.
 * @param lpszFunctionName Name of the ISR entry point.
 * @param bIRQ IRQ the handler services.
 * @return Handle to the installed handler, or NULL on failure.
 */
HANDLE WINAPI LoadIntChainHandler(LPCWSTR lpFilename, LPCWSTR lpszFunctionName, BYTE bIRQ);
/**
 * Loads a DLL into the kernel's address space.
 *
 * Intended for event tracking (CeLog) and kernel debugger DLLs. The DLL stays
 * loaded until the device restarts and cannot depend on other DLLs. Trusted
 * callers only. Windows CE 4.1 and later.
 *
 * @param lpszFileName Name of the DLL.
 * @return Handle to the library, or NULL on failure.
 */
HANDLE WINAPI LoadKernelLibrary(LPCWSTR lpszFileName);
/**
 * Checks that a buffer passed by the calling process is accessible to that
 * process, and maps it.
 *
 * Drivers call it on pointers received in IOCTLs so a caller cannot make the
 * driver read or write memory the caller has no access to. On CE 6.0 the
 * kernel validates IOCTL buffers itself. Windows CE 4.1 and later.
 *
 * @param ptr Pointer from the caller.
 * @param dwLen Length of the buffer in bytes.
 * @return The mapped pointer if the whole range is valid for the caller,
 *         otherwise NULL.
 */
LPVOID WINAPI MapCallerPtr(LPVOID ptr, DWORD dwLen);
/**
 * Mounts a partition by loading a file system on it.
 *
 * Before CE 6.0 this lived in storeapi.lib rather than coredll. Windows CE 6.0
 * and later.
 *
 * @param hPartition Partition handle from OpenPartition.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI MountPartition(HANDLE hPartition);
/**
 * Opens a partition on a store.
 *
 * Close the handle with CloseHandle. Before CE 6.0 this lived in storeapi.lib
 * rather than coredll. Windows CE 6.0 and later.
 *
 * @param hStore Store handle from OpenStore.
 * @param szPartitionName Name of the partition.
 * @return Partition handle, or INVALID_HANDLE_VALUE on failure.
 */
HANDLE WINAPI OpenPartition(HANDLE hStore, LPCWSTR szPartitionName);
/**
 * Opens a storage device, such as "DSK1:", for partition management.
 *
 * Microsoft's reference gives the parameter as LPCSTR, but the function takes
 * a wide string. Close the handle with CloseHandle. Before CE 6.0 this lived
 * in storeapi.lib rather than coredll. Windows CE 6.0 and later.
 *
 * @param szDeviceName Device name, for example L"DSK1:".
 * @return Store handle, or INVALID_HANDLE_VALUE on failure. Call GetLastError
 *         for details.
 */
HANDLE WINAPI OpenStore(LPCWSTR szDeviceName);
/**
 * Opens an existing named watchdog timer.
 *
 * Typically used by a monitor process to wait on another process's watchdog.
 * The handle is signaled when the watchdog is not refreshed within its
 * period. Watchdog timers have their own namespace, separate from events and
 * mutexes. Close with CloseHandle. Windows CE 6.0 and later.
 *
 * @param pszWatchDogName Name of the watchdog timer.
 * @param dwFlags Reserved; must be zero.
 * @return Watchdog handle, or NULL on failure.
 */
HANDLE WINAPI OpenWatchDogTimer(LPCWSTR pszWatchDogName, DWORD dwFlags);
/**
 * Discards the pageable pages of a process or DLL.
 *
 * Meant for shells that release memory held by inactive processes. Only call
 * it after the system has been idle for a while, or pages still in use get
 * thrown away and reloaded. Windows CE 4.2 and later.
 *
 * @param hModule Process or DLL handle.
 * @param dwFlags PAGE_OUT_PROCESS_ONLY for just this module,
 *        PAGE_OUT_DLL_USED_ONLY_BY_THISPROC to include DLLs only this process
 *        loaded (ignored for a DLL), or PAGE_OUT_ALL_DEPENDENT_DLL to include
 *        every dependent DLL, even ones shared with other processes.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI PageOutModule(HANDLE hModule, DWORD dwFlags);
/**
 * Asks the OEM profiler for a snapshot of its counters.
 *
 * The kernel calls IOCTL_HAL_OEM_PROFILER with PROFILE_OEM_QUERY and writes
 * the result into the CeLog stream as CELID_OEMPROFILER_HIT; nothing comes
 * back to the caller, so this is only useful with CeLog capture running. Some
 * OEM profilers need ProfileStartEx first. What is captured depends on the
 * BSP. Windows CE 4.2 and later.
 */
VOID WINAPI ProfileCaptureStatus(void);
/**
 * Gets the set of gestures enabled for a window or for the calling process.
 *
 * Lets an application save the current set before disabling gestures and
 * restore it later. A gesture disabled at process scope is not delivered to
 * any window in the process, whatever the window's own setting. Part of the
 * touch gesture component (SYSGEN_TOUCHGESTURE, CE 6.0 R3). Windows CE 6.0
 * and later.
 *
 * @param hwnd Window to query. Ignored for TGF_SCOPE_PROCESS.
 * @param uScope TGF_SCOPE_WINDOW or TGF_SCOPE_PROCESS.
 * @param pullFlags Receives the enabled gestures as TGF_ flags (TGF_PAN,
 *        TGF_SCROLL, TGF_HOLD, TGF_SELECT, TGF_DOUBLESELECT), plus
 *        GID_TO_TGF(id) bits for registered custom gestures.
 * @return TRUE on success, FALSE on failure; call GetLastError.
 */
BOOL WINAPI QueryGestures(HWND hwnd, UINT uScope, PULONGLONG pullFlags);
/**
 * Tests whether the running OS can execute a given instruction set, or
 * reports the one it uses.
 *
 * Meant for installers deciding whether a binary will run. Floating-point
 * variants (the ...FP_INSTRUCTION values) imply FPU hardware; the others run
 * with FP emulation. Windows CE 4.1 and later.
 *
 * @param dwInstructionSet A PROCESSOR_..._INSTRUCTION value to test, or
 *        PROCESSOR_QUERY_INSTRUCTION to ask for the current one.
 * @param lpdwCurrentInstructionSet Receives the OS's instruction set when
 *        dwInstructionSet is PROCESSOR_QUERY_INSTRUCTION.
 * @return TRUE if the instruction set is supported, or always for
 *         PROCESSOR_QUERY_INSTRUCTION. FALSE if unsupported or unknown.
 */
BOOL WINAPI QueryInstructionSet(DWORD dwInstructionSet, LPDWORD lpdwCurrentInstructionSet);
/**
 * Gets the connection-specific EAP data of a dial-up entry.
 *
 * CE keeps phone-book entries in the registry, so there is no phone-book
 * file. Windows CE 4.1 and later.
 *
 * @param pszPhonebook Ignored; pass NULL.
 * @param pszEntry Name of the entry.
 * @param pbEapData Buffer that receives the EAP data.
 * @param pdwSizeofEapData On input the size of pbEapData; receives the size
 *        needed if it is too small.
 * @return ERROR_SUCCESS, or an error code: ERROR_BUFFER_TOO_SMALL,
 *         E_INVALIDARG if pdwSizeofEapData is NULL,
 *         ERROR_CANNOT_FIND_PHONEBOOK_ENTRY or ERROR_CANNOT_OPEN_PHONEBOOK.
 */
DWORD WINAPI RasGetEapConnectionData(LPCWSTR pszPhonebook, LPCWSTR pszEntry, PBYTE pbEapData, PDWORD pdwSizeofEapData);
/**
 * Gets the user-specific EAP data stored for a dial-up entry.
 *
 * Windows CE 4.1 and later.
 *
 * @param hToken Unused on CE; pass NULL.
 * @param pszPhonebook Ignored; pass NULL.
 * @param pszEntry Name of an existing entry.
 * @param pbEapData Buffer that receives the EAP data.
 * @param pdwSizeofEapData On input the size of pbEapData; receives the size
 *        needed if it is too small.
 * @return ERROR_SUCCESS, or an error code: ERROR_BUFFER_TOO_SMALL,
 *         E_INVALIDARG if pdwSizeofEapData is NULL,
 *         ERROR_CANNOT_FIND_PHONEBOOK_ENTRY or ERROR_CANNOT_OPEN_PHONEBOOK.
 */
DWORD WINAPI RasGetEapUserData(HANDLE hToken, LPCWSTR pszPhonebook, LPCWSTR pszEntry, PBYTE pbEapData, PDWORD pdwSizeofEapData);
/**
 * Stores connection-specific EAP data for a dial-up entry.
 *
 * Microsoft's CE reference declares the size as a pointer, unlike the desktop
 * function, which takes it by value. Windows CE 4.1 and later.
 *
 * @param pszPhonebook Ignored; pass NULL.
 * @param pszEntry Name of the entry.
 * @param pbEapData EAP data to store.
 * @param pdwSizeofEapData Points to the size of pbEapData in bytes.
 * @return ERROR_SUCCESS, or an error code: E_INVALIDARG if pdwSizeofEapData
 *         is NULL, ERROR_CANNOT_FIND_PHONEBOOK_ENTRY or
 *         ERROR_CANNOT_OPEN_PHONEBOOK.
 */
DWORD WINAPI RasSetEapConnectionData(LPCWSTR pszPhonebook, LPCWSTR pszEntry, PBYTE pbEapData, PDWORD pdwSizeofEapData);
/**
 * Stores user-specific EAP data for a dial-up entry in the registry.
 *
 * Windows CE 4.1 and later.
 *
 * @param hToken Ignored; pass NULL.
 * @param pszPhonebook Ignored; pass NULL.
 * @param pszEntry Name of an existing entry.
 * @param pbEapData EAP data to store.
 * @param dwSizeofEapData Size of pbEapData in bytes.
 * @return ERROR_SUCCESS, or an error code: E_INVALIDARG if pbEapData is NULL
 *         or dwSizeofEapData is zero, ERROR_CANNOT_FIND_PHONEBOOK_ENTRY or
 *         ERROR_CANNOT_OPEN_PHONEBOOK.
 */
DWORD WINAPI RasSetEapUserData(HANDLE hToken, LPCWSTR pszPhonebook, LPCWSTR pszEntry, BYTE *pbEapData, DWORD dwSizeofEapData);
/**
 * Refreshes a watchdog timer so that its default action is not taken.
 *
 * The watched process must call this within each watchdog period. Refreshing a
 * timer that has not been started succeeds. Windows CE 6.0 and later.
 *
 * @param hWatchDog Watchdog handle.
 * @param dwFlags Reserved; must be zero.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details;
 *         ERROR_INVALID_HANDLE for a bad handle.
 */
BOOL WINAPI RefreshWatchDogTimer(HANDLE hWatchDog, DWORD dwFlags);
/**
 * Swaps in a saved system registry hive, to take effect at the next boot.
 *
 * Hive-based registry only; the object-store registry uses RegCopyFile and
 * RegRestoreFile instead. The system hive is unmounted immediately and the
 * registry is unusable until restart. The current hive is moved to lpOldFile;
 * lpNewFile, usually made by RegSaveKey, becomes the hive, or with NULL the
 * default hive is loaded from ROM. Only trusted callers may use it. To
 * restore a user hive, copy the file into the user's profile directory while
 * that user is logged out. Windows CE 4.1 and later.
 *
 * @param hKey Must be HKEY_LOCAL_MACHINE.
 * @param lpSubKey Must be NULL.
 * @param lpNewFile Saved hive to restore, or NULL for the ROM default.
 * @param lpOldFile File that receives the current hive, or NULL.
 * @return ERROR_SUCCESS on success, otherwise an error code such as
 *         ERROR_ACCESS_DENIED or ERROR_NOT_SUPPORTED.
 */
LONG WINAPI RegReplaceKey(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpNewFile, LPCWSTR lpOldFile);
/**
 * Saves a whole registry hive to a file.
 *
 * Hive-based registry only. Any key under HKEY_LOCAL_MACHINE,
 * HKEY_CLASSES_ROOT or HKEY_USERS saves the entire system hive; any key under
 * HKEY_CURRENT_USER saves the entire user hive. Volatile keys are not saved.
 * Restore with RegReplaceKey. Windows CE 4.1 and later.
 *
 * @param hKey An open key or a predefined root key.
 * @param lpFile Name of the file to create.
 * @param lpSecurityAttributes Must be NULL.
 * @return ERROR_SUCCESS on success, otherwise an error code such as
 *         ERROR_NOT_SUPPORTED.
 */
LONG WINAPI RegSaveKey(HKEY hKey, LPCWSTR lpFile, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
/**
 * Registers the shell's desktop window with the window manager.
 *
 * The window then receives notifications when top-level windows are created,
 * destroyed and activated. It must already exist, be full-screen and opaque,
 * and have the class name DesktopExplorerWindow. For shell replacements.
 * Windows CE 4.1 and later.
 *
 * @param hwndDesktop The desktop window.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI RegisterDesktop(HWND hwndDesktop);
/**
 * Registers a custom gesture name, or looks up the ID of one already
 * registered.
 *
 * Custom gesture recognizers call it during initialization; the ID is what
 * WM_GESTURE carries in wParam. Up to 63 gestures, core ones included, can
 * be registered, and they stay registered until reboot. Names must not clash
 * with the core gestures (Begin, End, Pan, Rotate, Scroll, Hold, Select,
 * DoubleSelect). Windows CE 6.0 and later.
 *
 * @param pszName Case-insensitive gesture name, up to 63 characters.
 * @param pdwID Receives the gesture ID.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI RegisterGesture(LPCWSTR pszName, PDWORD_PTR pdwID);
/**
 * Loads a Services.exe service DLL, or starts another instance of one already
 * loaded.
 *
 * The DLL must export xxx_Init, xxx_Deinit and xxx_IOControl. Each instance is
 * identified by its prefix and index, which must be unique. dwInfo is passed
 * to xxx_Init, and a zero return from it fails the call. Trusted callers
 * only; Microsoft recommends registry configuration plus ActivateService
 * instead. Windows CE 4.1 and later.
 *
 * @param lpszType Three-letter service prefix.
 * @param dwIndex Instance index, 0 to 9.
 * @param lpszLib Name of the service DLL.
 * @param dwInfo Value passed to the service's xxx_Init.
 * @return Service handle, or NULL on failure; GetLastError gives
 *         ERROR_FILE_NOT_FOUND, ERROR_INVALID_FUNCTION, ERROR_DEVICE_IN_USE or
 *         ERROR_OPEN_FAILED.
 */
HANDLE WINAPI RegisterService(LPCWSTR lpszType, DWORD dwIndex, LPCWSTR lpszLib, DWORD dwInfo);
/**
 * Registers the shell's taskbar window with the window manager.
 *
 * The window then receives notifications when top-level windows are created,
 * destroyed and activated. It must already exist. It can be registered only
 * once, but calling again updates the on-top state. For shell replacements.
 * Windows CE 4.1 and later.
 *
 * @param hwndTaskbar The taskbar window.
 * @param bTaskBarOnTop TRUE if the taskbar is kept on top.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI RegisterTaskBarEx(HWND hwndTaskbar, BOOL bTaskBarOnTop);
/**
 * Ends a power relationship set up by RegisterPowerRelationship.
 *
 * This also deregisters the child device from the Power Manager. Windows CE
 * 4.1 and later.
 *
 * @param h Handle returned by RegisterPowerRelationship.
 * @return ERROR_SUCCESS, or ERROR_INVALID_PARAMETER for a bad handle or a
 *         child that has already gone.
 */
DWORD WINAPI ReleasePowerRelationship(HANDLE h);
/**
 * Renames a partition.
 *
 * Before CE 6.0 this lived in storeapi.lib rather than coredll. Windows CE 6.0
 * and later.
 *
 * @param hPartition Partition handle from OpenPartition.
 * @param szNewName New partition name.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI RenamePartition(HANDLE hPartition, LPCWSTR szNewName);
/**
 * Asks the Bluetooth stack to post events of the given classes to a message
 * queue.
 *
 * Create hMsgQ with CreateMsgQueue for reading, sized for BTEVENT messages.
 * Stop with StopBluetoothNotifications. Windows CE 5.1 and later.
 *
 * @param dwClass Event classes: BTE_CLASS_CONNECTIONS, BTE_CLASS_PAIRING,
 *        BTE_CLASS_DEVICE and BTE_CLASS_STACK, or'd together.
 * @param hMsgQ Message queue that receives the events.
 * @return Notification handle, or NULL on failure.
 */
HANDLE WINAPI RequestBluetoothNotifications(DWORD dwClass, HANDLE hMsgQ);
/**
 * Asks for a message each time a device interface of a given class appears
 * or disappears.
 *
 * Each message is a DEVDETAIL structure followed by the interface name, up to
 * MAX_DEVCLASS_NAMELEN characters; create hMsgQ with CreateMsgQueue for
 * reading, with messages big enough for that. Wait on the queue, not on the
 * returned handle. Stop with StopDeviceNotifications. Windows CE 4.1 and
 * later.
 *
 * @param devclass Interface class GUID, or NULL for every class (wasteful;
 *        avoid).
 * @param hMsgQ Message queue that receives the notifications.
 * @param fAll TRUE to be sent the interfaces that already exist as well as
 *        later changes.
 * @return Notification handle, or NULL on failure.
 */
HANDLE WINAPI RequestDeviceNotifications(const GUID *devclass, HANDLE hMsgQ, BOOL fAll);
/**
 * Creates a pool of numbered resources, such as IRQs or I/O ranges, for the
 * I/O resource manager.
 *
 * Drivers then claim from the pool with ResourceRequest and return to it with
 * ResourceRelease. Windows CE 4.1 and later.
 *
 * @param dwResId Resource type: RESMGR_IRQ, RESMGR_IOSPACE, or an OEM value
 *        from RESMGR_BASE_OEM to RESMGR_MAX_OEM.
 * @param dwMinimum First resource number in the pool.
 * @param dwCount Number of resources in the pool.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI ResourceCreateList(DWORD dwResId, DWORD dwMinimum, DWORD dwCount);
/**
 * Destroys a resource pool created with ResourceCreateList.
 *
 * Trusted callers only. Windows CE 5.0 and later.
 *
 * @param dwResId Resource type passed to ResourceCreateList.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI ResourceDestroyList(DWORD dwResId);
/**
 * Marks a range of resources as shareable or not.
 *
 * Before CE 5.0 shared ranges could only be set in the registry. Windows CE
 * 5.0 and later.
 *
 * @param dwResId Resource type, such as RESMGR_IRQ.
 * @param dwId First resource in the range.
 * @param dwLen Number of resources in the range.
 * @param fShareable TRUE for shareable, FALSE for not.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI ResourceMarkAsShareable(DWORD dwResId, DWORD dwId, DWORD dwLen, BOOL fShareable);
/**
 * Returns a range of resources to the resource manager's pool.
 *
 * The whole range is released or none of it is. Windows CE 4.1 and later.
 *
 * @param dwResId Resource type, such as RESMGR_IRQ.
 * @param dwBase First resource in the range.
 * @param dwLen Number of resources in the range.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI ResourceRelease(DWORD dwResId, DWORD dwBase, DWORD dwLen);
/**
 * Claims a range of resources from the resource manager's pool.
 *
 * The whole range is claimed or none of it is. Bus drivers call it for the
 * drivers they load. Windows CE 4.1 and later.
 *
 * @param dwResId Resource type, such as RESMGR_IRQ.
 * @param dwBase First resource in the range.
 * @param dwLen Number of resources in the range.
 * @return TRUE on success, FALSE if any part of the range is unavailable.
 *
 * @note Windows CE 2.0 only.
 */
BOOL WINAPI ResourceRequest(DWORD dwResId, DWORD dwBase, DWORD dwLen);
/**
 * Claims a range of resources, optionally with exclusive use of a shareable
 * range.
 *
 * For devices that cannot share, such as network cards that need an IRQ to
 * themselves. Bus drivers make the request for the drivers they load.
 * Windows CE 5.0 and later.
 *
 * @param dwResId Resource type, such as RESMGR_IRQ.
 * @param dwId First resource in the range.
 * @param dwLen Number of resources in the range.
 * @param dwFlags RREXF_REQUEST_EXCLUSIVE for exclusive access.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI ResourceRequestEx(DWORD dwResId, DWORD dwId, DWORD dwLen, DWORD dwFlags);
/**
 * Sends a message to a window and waits for it to be processed, giving up
 * after a timeout.
 *
 * A window on the calling thread is called directly and the timeout is
 * ignored. With HWND_BROADCAST every top-level window gets the full timeout
 * in turn, so the total wait can be uTimeout times the number of windows.
 * SMTO_NORMAL is the only flag CE supports. Windows CE 4.1 and later.
 *
 * @param hWnd Target window, or HWND_BROADCAST.
 * @param Msg Message to send.
 * @param wParam Message-specific value.
 * @param lParam Message-specific value.
 * @param fuFlags SMTO_NORMAL.
 * @param uTimeout Timeout in milliseconds, per window.
 * @param lpdwResult Receives the result of the message processing.
 * @return Nonzero on success. Zero on failure or timeout; GetLastError
 *         returns 0 for a timeout. For a broadcast it does not report
 *         individual timeouts.
 */
LRESULT WINAPI SendMessageTimeout(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult);
/**
 * Sends an I/O control to a Services.exe service.
 *
 * Deprecated: open the service with CreateFile and use DeviceIoControl with
 * the same arguments. Windows CE 4.1 and later.
 *
 * @param hService Service handle, from GetServiceHandle.
 * @param dwIoControlCode Control code, such as IOCTL_SERVICE_STOP or a
 *        service-defined CTL_CODE value.
 * @param lpInBuf Input buffer, or NULL if the operation takes no input.
 * @param nInBufSize Size of lpInBuf in bytes.
 * @param lpOutBuf Output buffer, or NULL if the operation returns no data.
 * @param nOutBufSize Size of lpOutBuf in bytes.
 * @param lpBytesReturned Receives the number of bytes written to lpOutBuf.
 *        Used even when there is no output.
 * @param lpOverlapped Ignored; pass NULL.
 * @return Nonzero on success, zero on failure; call GetLastError.
 */
BOOL WINAPI ServiceIoControl(HANDLE hService, DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
/**
 * Stops the Services.exe super server listening on every port of a service.
 *
 * Windows CE 4.1 and later.
 *
 * @param hService Service instance whose ports to close.
 * @return Nonzero on success, zero on failure; call GetLastError.
 */
BOOL WINAPI ServiceUnbindPorts(HANDLE hService);
/**
 * Logs a user on, mounting their registry hive and profile, and logs off the
 * previous user.
 *
 * The secret is passed to the security subsystem to unlock the user's
 * credentials. On CE 6.0 this only switches the user registry; it does not
 * tell processes, drivers or services about the logon. To log everyone off,
 * pass NULL, NULL, 0, FALSE. Trusted callers only. Windows CE 4.1 and later.
 *
 * @param lpszUserName Logon name, case-insensitive. Must be a valid file name,
 *        as it names the profile directory.
 * @param lpbUserSecret Opaque secret data such as a password.
 * @param dwDataSize Size of lpbUserSecret in bytes.
 * @param bCreateIfNew TRUE to create the user if it does not exist.
 * @return TRUE if the user was logged on. FALSE with ERROR_USER_EXISTS if
 *         that user was already logged on, who stays logged on; on any other
 *         failure no user is left logged on.
 */
BOOL WINAPI SetCurrentUser(LPCWSTR lpszUserName, LPBYTE lpbUserSecret, DWORD dwDataSize, BOOL bCreateIfNew);
/**
 * Attaches a DWORD of data to an event, for the waiter to read with
 * GetEventData.
 *
 * The value can be a pointer, but it must be valid where it is dereferenced.
 * If it is set twice before the waiter reads it, the first value is lost.
 * Requires privileged access on CE 6.0. Windows CE 4.1 and later.
 *
 * @param hEvent Event handle from CreateEvent.
 * @param dwData Value to attach.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details.
 */
BOOL WINAPI SetEventData(HANDLE hEvent, DWORD dwData);
/**
 * Sets the kernel's low-memory thresholds and the event it signals when they
 * are crossed.
 *
 * Below the thresholds the kernel pages out discardable code, reclaims unused
 * stack and limits VirtualAlloc block sizes. Without a call all thresholds
 * are zero and none of this happens. Images with the OOM component call it
 * from registry settings, so only call it yourself without that component.
 * On the event, check GlobalMemoryStatus to see which level was reached.
 * Replaces SetGwesOOMEvent. Windows CE 4.1 and later.
 *
 * @param hEvent Event the kernel sets when memory gets low or critical.
 * @param cpLow Free pages below which memory is low.
 * @param cpCritical Free pages below which memory is critical.
 * @param cpLowBlockSize Largest VirtualAlloc block, in pages, allowed when
 *        it would take free memory below cpLow.
 * @param cpCriticalBlockSize Largest VirtualAlloc block, in pages, allowed
 *        when it would take free memory below cpCritical.
 *
 * @note Windows CE 3.0 and later.
 */
void WINAPI SetOOMEvent(HANDLE hEvent, DWORD cpLow, DWORD cpCritical, DWORD cpLowBlockSize, DWORD cpCriticalBlockSize);
/**
 * Sets the attributes of a partition.
 *
 * Before CE 6.0 this lived in storeapi.lib rather than coredll. Windows CE 6.0
 * and later.
 *
 * @param hPartition Partition handle from OpenPartition.
 * @param dwAttrs New attribute flags.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI SetPartitionAttributes(HANDLE hPartition, DWORD dwAttrs);
/**
 * Turns the system password on or off without changing it.
 *
 * lpszPassword must match the current password. Needs the password component
 * (Fspass) in the image. Windows CE 4.1 and later.
 *
 * @param dwStatus Zero, or PASSWORD_STATUS_ACTIVE optionally with
 *        PASSWORD_STATUS_SCREENSAVERPROTECT to also ask for it after the
 *        screen saver.
 * @param lpszPassword The current password.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI SetPasswordStatus(DWORD dwStatus, LPWSTR lpszPassword);
/**
 * Changes the secret, such as a password, that protects the logged-on user's
 * credentials.
 *
 * The data goes to the security subsystem, which uses it to encrypt and
 * decrypt the user's credentials. Fails with ERROR_FILE_NOT_FOUND if no user
 * is logged on. Trusted callers only. Windows CE 4.1 and later.
 *
 * @param lpbUserData New opaque secret data.
 * @param dwDataSize Size of lpbUserData in bytes.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI SetUserData(LPBYTE lpbUserData, DWORD dwDataSize);
/**
 * Sets the current user's default locale.
 *
 * Afterwards, broadcast the change with PostMessage(HWND_BROADCAST,
 * WM_WININICHANGE, 0, INI_INTL). Running applications may not pick up the new
 * locale until restarted. On CE 6.0 the caller needs OEM_CERTIFY_TRUST or the
 * call fails with ERROR_ACCESS_DENIED. Windows CE 4.2 and later.
 *
 * @param Locale Locale identifier to make the user default.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI SetUserDefaultLCID(LCID Locale);
/**
 * Sets the current user's UI language.
 *
 * Takes effect only after the device is reset. Windows CE 4.21 and later.
 *
 * @param langid Language identifier for the UI language.
 * @return TRUE on success, FALSE on failure.
 *
 * @note Windows CE 3.0 and later.
 */
BOOL WINAPI SetUserDefaultUILanguage(LANGID langid);
/**
 * Blocks the calling thread until the next system tick.
 *
 * Gives up the rest of the thread's time slice. Windows CE 4.1 and later.
 */
void WINAPI SleepTillTick(void);
/**
 * Starts a watchdog timer, making the calling process the one watched.
 *
 * If the timer's default action is WDOG_KILL_PROCESS, the calling process is
 * the one terminated. Windows CE 6.0 and later.
 *
 * @param hWatchDog Watchdog handle.
 * @param dwFlags Reserved; must be zero.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details;
 *         ERROR_INVALID_HANDLE for a bad handle.
 */
BOOL WINAPI StartWatchDogTimer(HANDLE hWatchDog, DWORD dwFlags);
/**
 * Stops Bluetooth stack events requested with RequestBluetoothNotifications.
 *
 * Windows CE 5.1 and later.
 *
 * @param h Handle returned by RequestBluetoothNotifications.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI StopBluetoothNotifications(HANDLE h);
/**
 * Stops device interface notifications requested with
 * RequestDeviceNotifications.
 *
 * Once it returns, the message queue can be closed. Windows CE 4.1 and later.
 *
 * @param h Handle returned by RequestDeviceNotifications.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI StopDeviceNotifications(HANDLE h);
/**
 * Stops a watchdog timer.
 *
 * Stopping a timer that has signaled but not yet taken its default action
 * cancels the action. Windows CE 6.0 and later.
 *
 * @param hWatchDog Watchdog handle.
 * @param dwFlags Reserved; must be zero.
 * @return TRUE on success, FALSE on failure. Call GetLastError for details;
 *         ERROR_INVALID_HANDLE for a bad handle.
 */
BOOL WINAPI StopWatchDogTimer(HANDLE hWatchDog, DWORD dwFlags);
/**
 * Reads a property of a waveform audio input device or stream.
 *
 * Properties are grouped into sets named by GUID, and drivers may define their
 * own. The predefined sets cover audio gain classes: MM_PROP_GAINCLASS_CLASS
 * returns an AUDIOGAINCLASS and MM_PROP_GAINCLASS_STREAM a STREAMPROPS.
 * Windows CE 5.0 and later.
 *
 * @param uDeviceID Device ID, or an open HWAVEIN or HWAVEOUT cast to UINT.
 *        Must be 0 for MM_PROPSET_GAINCLASS_CLASS.
 * @param pPropSetId GUID of the property set, such as
 *        MM_PROPSET_GAINCLASS_CLASS or MM_PROPSET_GAINCLASS_STREAM.
 * @param ulPropId Property within the set.
 * @param pvPropParams Extra data qualifying the property, such as the gain
 *        class ID for MM_PROP_GAINCLASS_CLASS, or NULL.
 * @param cbPropParams Size of pvPropParams in bytes.
 * @param pvPropData Receives the property value.
 * @param cbPropData Size of pvPropData in bytes.
 * @param pcbReturn Receives the number of bytes written to pvPropData.
 * @return MMSYSERR_NOERROR on success, otherwise MMSYSERR_INVALPARAM,
 *         MMSYSERR_NODRIVER or a driver-specific error.
 */
MMRESULT WINAPI waveInGetProperty(UINT uDeviceID, const GUID *pPropSetId, ULONG ulPropId, LPVOID pvPropParams, ULONG cbPropParams, LPVOID pvPropData, ULONG cbPropData, PULONG pcbReturn);
/**
 * Sets a property of a waveform audio input device or stream.
 *
 * See waveInGetProperty for property sets. Windows CE 5.0 and later.
 *
 * @param uDeviceID Device ID, or an open HWAVEIN or HWAVEOUT cast to UINT.
 *        Must be 0 for MM_PROPSET_GAINCLASS_CLASS.
 * @param pPropSetId GUID of the property set, such as
 *        MM_PROPSET_GAINCLASS_CLASS or MM_PROPSET_GAINCLASS_STREAM.
 * @param ulPropId Property within the set.
 * @param pvPropParams Extra data qualifying the property, or NULL.
 * @param cbPropParams Size of pvPropParams in bytes.
 * @param pvPropData New property value.
 * @param cbPropData Size of pvPropData in bytes.
 * @return MMSYSERR_NOERROR on success, otherwise MMSYSERR_INVALPARAM,
 *         MMSYSERR_NODRIVER or a driver-specific error.
 */
MMRESULT WINAPI waveInSetProperty(UINT uDeviceID, const GUID *pPropSetId, ULONG ulPropId, LPVOID pvPropParams, ULONG cbPropParams, LPVOID pvPropData, ULONG cbPropData);
/**
 * Reads a property of a waveform audio output device or stream.
 *
 * Properties are grouped into sets named by GUID, and drivers may define their
 * own. The predefined sets cover audio gain classes: MM_PROP_GAINCLASS_CLASS
 * returns an AUDIOGAINCLASS and MM_PROP_GAINCLASS_STREAM a STREAMPROPS.
 * Windows CE 5.0 and later.
 *
 * @param uDeviceID Device ID, or an open HWAVEIN or HWAVEOUT cast to UINT.
 *        Must be 0 for MM_PROPSET_GAINCLASS_CLASS.
 * @param pPropSetId GUID of the property set, such as
 *        MM_PROPSET_GAINCLASS_CLASS or MM_PROPSET_GAINCLASS_STREAM.
 * @param ulPropId Property within the set.
 * @param pvPropParams Extra data qualifying the property, such as the gain
 *        class ID for MM_PROP_GAINCLASS_CLASS, or NULL.
 * @param cbPropParams Size of pvPropParams in bytes.
 * @param pvPropData Receives the property value.
 * @param cbPropData Size of pvPropData in bytes.
 * @param pcbReturn Receives the number of bytes written to pvPropData.
 * @return MMSYSERR_NOERROR on success, otherwise MMSYSERR_INVALPARAM,
 *         MMSYSERR_NODRIVER or a driver-specific error.
 */
MMRESULT WINAPI waveOutGetProperty(UINT uDeviceID, const GUID *pPropSetId, ULONG ulPropId, LPVOID pvPropParams, ULONG cbPropParams, LPVOID pvPropData, ULONG cbPropData, PULONG pcbReturn);
/**
 * Sets a property of a waveform audio output device or stream.
 *
 * See waveOutGetProperty for property sets. Windows CE 5.0 and later.
 *
 * @param uDeviceID Device ID, or an open HWAVEIN or HWAVEOUT cast to UINT.
 *        Must be 0 for MM_PROPSET_GAINCLASS_CLASS.
 * @param pPropSetId GUID of the property set, such as
 *        MM_PROPSET_GAINCLASS_CLASS or MM_PROPSET_GAINCLASS_STREAM.
 * @param ulPropId Property within the set.
 * @param pvPropParams Extra data qualifying the property, or NULL.
 * @param cbPropParams Size of pvPropParams in bytes.
 * @param pvPropData New property value.
 * @param cbPropData Size of pvPropData in bytes.
 * @return MMSYSERR_NOERROR on success, otherwise MMSYSERR_INVALPARAM,
 *         MMSYSERR_NODRIVER or a driver-specific error.
 */
MMRESULT WINAPI waveOutSetProperty(UINT uDeviceID, const GUID *pPropSetId, ULONG ulPropId, LPVOID pvPropParams, ULONG cbPropParams, LPVOID pvPropData, ULONG cbPropData);

#ifdef __cplusplus
}
#endif

#endif

#endif
