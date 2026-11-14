#include <windows.h>
#include <csmedia.h>
#include <pm.h>
#include <stddef.h>

_Static_assert(sizeof(CSPROPERTY) == 24, "CSPROPERTY");
_Static_assert(sizeof(POWER_CAPABILITIES) == 48, "POWER_CAPABILITIES");
_Static_assert(sizeof(CSP_PIN) == 32 && offsetof(CSP_PIN, PinId) == 0x18, "CSP_PIN");
_Static_assert(sizeof(CSPROPERTY_STREAMEX_S) == 40 && offsetof(CSPROPERTY_STREAMEX_S, hMsgQueue) == 0x20, "CSPROPERTY_STREAMEX_S");
_Static_assert(sizeof(CSPROPERTY_VIDEOPROCAMP_S) == 40 && offsetof(CSPROPERTY_VIDEOPROCAMP_S, Capabilities) == 0x20, "VIDEOPROCAMP");
_Static_assert(sizeof(CSPROPERTY_CAMERACONTROL_S) == 40, "CAMERACONTROL");
_Static_assert(sizeof(CSPROPERTY_VIDEOCONTROL_CAPS_S) == 32 && sizeof(CSPROPERTY_VIDEOCONTROL_MODE_S) == 32, "VIDEOCONTROL");
_Static_assert(sizeof(CSPROPERTY_DROPPEDFRAMES_CURRENT_S) == 48 && offsetof(CSPROPERTY_DROPPEDFRAMES_CURRENT_S, AverageFrameSize) == 0x28, "DROPPEDFRAMES");
_Static_assert(sizeof(CSPROPERTY_DESCRIPTION) == 40 && offsetof(CSPROPERTY_DESCRIPTION, MembersListCount) == 0x20, "DESCRIPTION");
_Static_assert(sizeof(CSPROPERTY_MEMBERSHEADER) == 16 && sizeof(CSPROPERTY_STEPPING_LONG) == 16, "MEMBERS");
_Static_assert(sizeof(CSALLOCATOR_FRAMING) == 24, "CSALLOCATOR_FRAMING");
_Static_assert(sizeof(CSMULTIPLE_ITEM) == 8, "CSMULTIPLE_ITEM");
_Static_assert(sizeof(CSDATAFORMAT) == 64, "CSDATAFORMAT");
_Static_assert(sizeof(CS_VIDEO_STREAM_CONFIG_CAPS) == 128 && offsetof(CS_VIDEO_STREAM_CONFIG_CAPS, MinFrameInterval) == 0x68 &&
                   offsetof(CS_VIDEO_STREAM_CONFIG_CAPS, MinBitsPerSecond) == 0x78, "CONFIG_CAPS");
_Static_assert(sizeof(CS_VIDEOINFOHEADER) == 1128 && offsetof(CS_VIDEOINFOHEADER, AvgTimePerFrame) == 0x28 &&
                   offsetof(CS_VIDEOINFOHEADER, bmiHeader) == 0x30 && offsetof(CS_VIDEOINFOHEADER, dwBitMasks) == 0x58, "VIDEOINFOHEADER");
_Static_assert(sizeof(CS_DATAFORMAT_VIDEOINFOHEADER) == 1192 && offsetof(CS_DATAFORMAT_VIDEOINFOHEADER, VideoInfoHeader.bmiHeader) == 0x70,
               "DATAFORMAT_VIDEOINFOHEADER");
_Static_assert(sizeof(CS_DATARANGE_VIDEO) == 1336 && offsetof(CS_DATARANGE_VIDEO, ConfigCaps) == 0x50 &&
                   offsetof(CS_DATARANGE_VIDEO, VideoInfoHeader) == 0xD0, "DATARANGE_VIDEO");
_Static_assert(sizeof(CSSTREAM_HEADER) == 56 && offsetof(CSSTREAM_HEADER, Duration) == 0x18 && offsetof(CSSTREAM_HEADER, Data) == 0x28 &&
                   offsetof(CSSTREAM_HEADER, Handle) == 0x30, "CSSTREAM_HEADER");
_Static_assert(sizeof(CS_FRAME_INFO) == 64 && offsetof(CS_FRAME_INFO, PictureNumber) == 8, "CS_FRAME_INFO");
_Static_assert(sizeof(CS_STREAM_DESCRIPTOR) == 120 && offsetof(CS_STREAM_DESCRIPTOR, CsFrameInfo) == 56, "CS_STREAM_DESCRIPTOR");
_Static_assert(sizeof(CS_MSGQUEUE_BUFFER) == 16 && offsetof(CS_MSGQUEUE_BUFFER, pStreamDescriptor) == 12, "CS_MSGQUEUE_BUFFER");

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPWSTR command_line, int show) {
    HANDLE camera = CreateFileW(L"CAM1:", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (camera == INVALID_HANDLE_VALUE) {
        return 1;
    }
    CSP_PIN pin = {0};
    pin.Property.Set = CSPROPSETID_Pin;
    pin.Property.Id = CSPROPERTY_PIN_CTYPES;
    pin.Property.Flags = CSPROPERTY_TYPE_GET;
    ULONG pins = 0;
    DWORD returned = 0;
    BOOL ok = DeviceIoControl(camera, IOCTL_CS_PROPERTY, &pin, sizeof pin, &pins, sizeof pins, &returned, NULL);
    WCHAR text[64];
    wsprintfW(text, L"%d pins", ok ? (int)pins : -1);
    MessageBoxW(NULL, text, L"CAM1:", MB_OK);
    CloseHandle(camera);
    return ok ? 0 : 2;
}
