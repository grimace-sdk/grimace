#ifndef GRIMACE_MSGQUEUE_H
#define GRIMACE_MSGQUEUE_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MSGQUEUE_NOPRECOMMIT 0x00000001
#define MSGQUEUE_ALLOW_BROKEN 0x00000002
#define MSGQUEUE_MSGALERT 0x00000001

typedef struct MSGQUEUEOPTIONS_OS {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwMaxMessages;
    DWORD cbMaxMessage;
    BOOL bReadAccess;
} MSGQUEUEOPTIONS, *LPMSGQUEUEOPTIONS, *PMSGQUEUEOPTIONS;

typedef struct MSGQUEUEINFO {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwMaxMessages;
    DWORD cbMaxMessage;
    DWORD dwCurrentMessages;
    DWORD dwMaxQueueMessages;
    WORD wNumReaders;
    WORD wNumWriters;
} MSGQUEUEINFO, *LPMSGQUEUEINFO, *PMSGQUEUEINFO;

/**
 * Creates or opens a point-to-point message queue.
 *
 * Each handle is either a read or a write end, set by bReadAccess in
 * lpOptions. A queue handle can be waited on: read ends signal when a
 * message is available and write ends when there is room. Close with
 * CloseMsgQueue. Windows CE 4.1 and later.
 *
 * @param lpName Queue name, or NULL for an unnamed queue shared with
 *        OpenMsgQueue.
 * @param lpOptions MSGQUEUEOPTIONS with dwSize set, giving flags, maximum
 *        message count and size, and the access wanted.
 * @return Queue handle, or NULL on failure. GetLastError returns
 *         ERROR_ALREADY_EXISTS when a named queue was opened.
 */
HANDLE WINAPI CreateMsgQueue(LPCWSTR lpName, LPMSGQUEUEOPTIONS lpOptions);
/**
 * Opens a handle to a message queue created in another process.
 *
 * Use it for unnamed queues, whose handles can't be found by name. Close
 * with CloseMsgQueue. Windows CE 4.1 and later.
 *
 * @param hSrcProc Process that owns hMsgQ.
 * @param hMsgQ Queue handle, valid in hSrcProc.
 * @param lpOptions MSGQUEUEOPTIONS with dwSize and bReadAccess set.
 * @return Queue handle, or NULL on failure.
 */
HANDLE WINAPI OpenMsgQueue(HANDLE hSrcProc, HANDLE hMsgQ, LPMSGQUEUEOPTIONS lpOptions);
/**
 * Reads the next message from a message queue, waiting if it is empty.
 *
 * Windows CE 4.1 and later.
 *
 * @param hMsgQ Queue handle opened for reading.
 * @param lpBuffer Receives the message.
 * @param cbBufferSize Size of lpBuffer in bytes.
 * @param lpNumberOfBytesRead Receives the message size.
 * @param dwTimeout Milliseconds to wait, 0 or INFINITE.
 * @param pdwFlags Receives MSGQUEUE_MSGALERT if the message was written as
 *        an alert.
 * @return Nonzero on success, zero on failure. GetLastError returns
 *         ERROR_TIMEOUT, ERROR_INSUFFICIENT_BUFFER or
 *         ERROR_PIPE_NOT_CONNECTED when there are no writers.
 */
BOOL WINAPI ReadMsgQueue(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbBufferSize, LPDWORD lpNumberOfBytesRead, DWORD dwTimeout, DWORD *pdwFlags);
/**
 * Reads the next message from a message queue and identifies the writer.
 *
 * As ReadMsgQueue, with a security token for the process that wrote the
 * message. Windows CE 6.0 and later.
 *
 * @param hMsgQ Queue handle opened for reading.
 * @param lpBuffer Receives the message.
 * @param cbBufferSize Size of lpBuffer in bytes.
 * @param lpNumberOfBytesRead Receives the message size.
 * @param dwTimeout Milliseconds to wait, 0 or INFINITE.
 * @param pdwFlags Receives MSGQUEUE_MSGALERT if the message was written as
 *        an alert.
 * @param phTok Receives the writer's token handle, or NULL if not wanted.
 * @return Nonzero on success, zero on failure. Call GetLastError for
 *         details.
 */
BOOL WINAPI ReadMsgQueueEx(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbBufferSize, LPDWORD lpNumberOfBytesRead, DWORD dwTimeout, DWORD *pdwFlags, PHANDLE phTok);
/**
 * Writes a message to a message queue, waiting if it is full.
 *
 * Windows CE 4.1 and later.
 *
 * @param hMsgQ Queue handle opened for writing.
 * @param lpBuffer Message data.
 * @param cbDataSize Size of the message in bytes, up to the queue's
 *        maximum.
 * @param dwTimeout Milliseconds to wait for room, 0 or INFINITE.
 * @param dwFlags MSGQUEUE_MSGALERT to send it as an alert, or 0.
 * @return Nonzero on success, zero on failure. GetLastError returns
 *         ERROR_TIMEOUT, ERROR_INSUFFICIENT_BUFFER or
 *         ERROR_PIPE_NOT_CONNECTED when there are no readers.
 */
BOOL WINAPI WriteMsgQueue(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbDataSize, DWORD dwTimeout, DWORD dwFlags);
/**
 * Gets the settings and current state of a message queue.
 *
 * Windows CE 4.1 and later.
 *
 * @param hMsgQ Queue handle.
 * @param lpInfo MSGQUEUEINFO with dwSize set. Receives the information,
 *        including the messages queued and the numbers of readers and
 *        writers.
 * @return Nonzero on success, zero on failure.
 */
BOOL WINAPI GetMsgQueueInfo(HANDLE hMsgQ, LPMSGQUEUEINFO lpInfo);
/**
 * Closes a handle to a point-to-point message queue.
 *
 * The queue is destroyed when its last handle is closed. Windows CE 4.1
 * and later.
 *
 * @param hMsgQ Handle from CreateMsgQueue or OpenMsgQueue.
 * @return Nonzero on success, zero on failure.
 */
BOOL WINAPI CloseMsgQueue(HANDLE hMsgQ);

#ifdef __cplusplus
}
#endif

#endif
