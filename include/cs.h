#ifndef GRIMACE_CS_H
#define GRIMACE_CS_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CS_GUID(name, a, b, c, d0, d1, d2, d3, d4, d5, d6, d7) \
    static const GUID name __attribute__((unused)) = {a, b, c, {d0, d1, d2, d3, d4, d5, d6, d7}}

#define IOCTL_CS_PROPERTY 0x002F0003
#define IOCTL_STREAM_INSTANTIATE 0x002F001F
#define IOCTL_CS_BUFFERS 0x002F401F

typedef struct {
    union {
        struct {
            GUID Set;
            ULONG Id;
            ULONG Flags;
        };
        LONGLONG Alignment;
    };
} CSIDENTIFIER, *PCSIDENTIFIER;

typedef CSIDENTIFIER CSPROPERTY, *PCSPROPERTY;

#define CSPROPERTY_TYPE_GET 0x00000001
#define CSPROPERTY_TYPE_SET 0x00000002
#define CSPROPERTY_TYPE_SETSUPPORT 0x00000100
#define CSPROPERTY_TYPE_BASICSUPPORT 0x00000200
#define CSPROPERTY_TYPE_DEFAULTVALUES 0x00010000

typedef struct {
    ULONG Size;
    ULONG Count;
} CSMULTIPLE_ITEM, *PCSMULTIPLE_ITEM;

typedef struct {
    CSPROPERTY Property;
    ULONG PinId;
    ULONG Reserved;
} CSP_PIN, *PCSP_PIN;

typedef struct {
    ULONG AccessFlags;
    ULONG DescriptionSize;
    CSIDENTIFIER PropTypeSet;
    ULONG MembersListCount;
    ULONG Reserved;
} CSPROPERTY_DESCRIPTION, *PCSPROPERTY_DESCRIPTION;

#define CSPROPERTY_MEMBER_RANGES 0x00000001
#define CSPROPERTY_MEMBER_STEPPEDRANGES 0x00000002
#define CSPROPERTY_MEMBER_VALUES 0x00000003
#define CSPROPERTY_MEMBER_FLAG_DEFAULT 0x00000001

typedef struct {
    ULONG MembersFlags;
    ULONG MembersSize;
    ULONG MembersCount;
    ULONG Flags;
} CSPROPERTY_MEMBERSHEADER, *PCSPROPERTY_MEMBERSHEADER;

typedef struct {
    ULONG SteppingDelta;
    ULONG Reserved;
    struct {
        LONG SignedMinimum;
        LONG SignedMaximum;
    } Bounds;
} CSPROPERTY_STEPPING_LONG, *PCSPROPERTY_STEPPING_LONG;

CS_GUID(CSPROPTYPESETID_General, 0x97E99BA0, 0xBDEA, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00);

CS_GUID(CSPROPSETID_Pin, 0x8C134960, 0x51AD, 0x11CF, 0x87, 0x8A, 0x94, 0xF8, 0x01, 0xC1, 0x00, 0x00);

typedef enum {
    CSPROPERTY_PIN_CINSTANCES = 0,
    CSPROPERTY_PIN_CTYPES,
    CSPROPERTY_PIN_DATARANGES,
    CSPROPERTY_PIN_DATAINTERSECTION,
    CSPROPERTY_PIN_CATEGORY,
    CSPROPERTY_PIN_NAME,
    CSPROPERTY_PIN_DEVICENAME
} CSPROPERTY_PIN;

typedef struct {
    ULONG PossibleCount;
    ULONG CurrentCount;
} CSPIN_CINSTANCES, *PCSPIN_CINSTANCES;

CS_GUID(CSPROPSETID_Connection, 0x1D58C920, 0xAC9B, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00);

typedef enum {
    CSPROPERTY_CONNECTION_STATE = 0,
    CSPROPERTY_CONNECTION_DATAFORMAT,
    CSPROPERTY_CONNECTION_ALLOCATORFRAMING
} CSPROPERTY_CONNECTION;

typedef enum {
    CSSTATE_STOP = 0,
    CSSTATE_PAUSE,
    CSSTATE_RUN
} CSSTATE, *PCSSTATE;

#define CSPROPERTY_BUFFER_DRIVER 0x00000001
#define CSPROPERTY_BUFFER_CLIENT_LIMITED 0x00000002
#define CSPROPERTY_BUFFER_CLIENT_UNLIMITED 0x00000004

typedef struct {
    union {
        ULONG OptionsFlags;
        ULONG RequirementsFlags;
    };
    ULONG PoolType;
    ULONG Frames;
    ULONG FrameSize;
    union {
        ULONG FileAlignment;
        LONG FramePitch;
    };
    ULONG Reserved;
} CSALLOCATOR_FRAMING, *PCSALLOCATOR_FRAMING;

CS_GUID(CSPROPSETID_StreamEx, 0x443A0706, 0x9554, 0x41F4, 0xBB, 0x70, 0xEC, 0xB5, 0xD2, 0x10, 0xFB, 0x3B);

typedef enum {
    CSPROPERTY_STREAMEX_INIT = 0
} CSPROPERTY_STREAMEX;

typedef struct {
    CSP_PIN CsPin;
    HANDLE hMsgQueue;
} CSPROPERTY_STREAMEX_S, *PCSPROPERTY_STREAMEX_S;

typedef enum {
    CS_ALLOCATE = 0,
    CS_ENQUEUE,
    CS_DEALLOCATE
} BUFFER_COMMANDS;

typedef struct {
    union {
        struct {
            ULONG FormatSize;
            ULONG Flags;
            ULONG SampleSize;
            ULONG Reserved;
            GUID MajorFormat;
            GUID SubFormat;
            GUID Specifier;
        };
        LONGLONG Alignment;
    };
} CSDATAFORMAT, *PCSDATAFORMAT, CSDATARANGE, *PCSDATARANGE;

typedef struct {
    LONGLONG Time;
    ULONG Numerator;
    ULONG Denominator;
} CSTIME, *PCSTIME;

#define CSSTREAM_HEADER_OPTIONSF_SPLICEPOINT 0x00000001
#define CSSTREAM_HEADER_OPTIONSF_PREROLL 0x00000002
#define CSSTREAM_HEADER_OPTIONSF_DATADISCONTINUITY 0x00000004
#define CSSTREAM_HEADER_OPTIONSF_TYPECHANGED 0x00000008
#define CSSTREAM_HEADER_OPTIONSF_TIMEVALID 0x00000010
#define CSSTREAM_HEADER_OPTIONSF_TIMEDISCONTINUITY 0x00000040
#define CSSTREAM_HEADER_OPTIONSF_FLUSHONPAUSE 0x00000080
#define CSSTREAM_HEADER_OPTIONSF_DURATIONVALID 0x00000100
#define CSSTREAM_HEADER_OPTIONSF_ENDOFSTREAM 0x00000200

typedef struct {
    ULONG Size;
    ULONG TypeSpecificFlags;
    CSTIME PresentationTime;
    LONGLONG Duration;
    ULONG FrameExtent;
    ULONG DataUsed;
    PVOID Data;
    ULONG OptionsFlags;
    ULONG Handle;
} CSSTREAM_HEADER, *PCSSTREAM_HEADER;

typedef struct {
    ULONG ExtendedHeaderSize;
    DWORD dwFrameFlags;
    LONGLONG PictureNumber;
    LONGLONG DropCount;
    HANDLE hDirectDraw;
    HANDLE hSurfaceHandle;
    RECT DirectDrawRect;
    DWORD Reserved1;
    DWORD Reserved2;
    DWORD Reserved3;
    DWORD Reserved4;
} CS_FRAME_INFO, *PCS_FRAME_INFO;

typedef struct {
    CSSTREAM_HEADER CsStreamHeader;
    CS_FRAME_INFO CsFrameInfo;
} CS_STREAM_DESCRIPTOR, *PCS_STREAM_DESCRIPTOR;

#define FLAG_MSGQ_FRAME_BUFFER 0x00000001

typedef struct {
    DWORD Size;
    DWORD Flags;
    HANDLE Context;
} CS_MSGQUEUE_HEADER, *PCS_MSGQUEUE_HEADER;

typedef struct {
    CS_MSGQUEUE_HEADER CsMsgQueueHeader;
    PCS_STREAM_DESCRIPTOR pStreamDescriptor;
} CS_MSGQUEUE_BUFFER, *PCS_MSGQUEUE_BUFFER;

#ifdef __cplusplus
}
#endif

#endif
