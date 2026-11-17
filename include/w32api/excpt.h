#ifndef EXCPT_H
#define EXCPT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _EXCEPTION_DISPOSITION {
    ExceptionContinueExecution,
    ExceptionContinueSearch,
    ExceptionNestedException,
    ExceptionCollidedUnwind,
    ExceptionExecuteHandler
} EXCEPTION_DISPOSITION;

struct _CONTEXT;
typedef struct _CONTEXT CONTEXT;
struct _EXCEPTION_RECORD;
struct _EXCEPTION_POINTERS;
typedef struct _CONTEXT *PCONTEXT;
/**
 * Pointer to a EXCEPTION_RECORD.
 *
 * See EXCEPTION_RECORD: This structure describes an exception.
 */
typedef struct _EXCEPTION_RECORD *PEXCEPTION_RECORD;
/**
 * Pointer to a EXCEPTION_POINTERS.
 *
 * See EXCEPTION_POINTERS: This structure contains an exception record with a machine-independent description of an exception and a context record with a machine-dependent description of the processor context at the time of the exception.
 */
typedef struct _EXCEPTION_POINTERS *PEXCEPTION_POINTERS;
typedef PCONTEXT LPCONTEXT;
/**
 * Pointer to a EXCEPTION_RECORD.
 *
 * See EXCEPTION_RECORD: This structure describes an exception.
 */
typedef PEXCEPTION_RECORD LPEXCEPTION_RECORD;
/**
 * Pointer to a EXCEPTION_POINTERS.
 *
 * See EXCEPTION_POINTERS: This structure contains an exception record with a machine-independent description of an exception and a context record with a machine-dependent description of the processor context at the time of the exception.
 */
typedef PEXCEPTION_POINTERS LPEXCEPTION_POINTERS;
struct _RUNTIME_FUNCTION;

#if defined(_WIN32_WCE) && defined(_X86_)
typedef struct _DISPATCHER_CONTEXT {
    struct _EXCEPTION_REGISTRATION_RECORD *RegistrationPointer;
    unsigned long ControlPc;
} DISPATCHER_CONTEXT, *PDISPATCHER_CONTEXT;
#else
typedef struct _DISPATCHER_CONTEXT {
    unsigned long ControlPc;
    struct _RUNTIME_FUNCTION *FunctionEntry;
    unsigned long EstablisherFrame;
    CONTEXT* ContextRecord;
} DISPATCHER_CONTEXT, *PDISPATCHER_CONTEXT;
#endif

#define EXCEPTION_EXECUTE_HANDLER        1
#define EXCEPTION_CONTINUE_SEARCH        0
#define EXCEPTION_CONTINUE_EXECUTION    -1

#ifdef __cplusplus
}
#endif

#endif
