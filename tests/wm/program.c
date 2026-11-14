#include <windows.h>
#include <aygshell.h>
#include <pm.h>

static LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    static SHACTIVATEINFO activation = {sizeof activation};
    switch (message) {
    case WM_ACTIVATE:
        SHHandleWMActivate(window, wparam, lparam, &activation, 0);
        return 0;
    case WM_SETTINGCHANGE:
        SHHandleWMSettingChange(window, wparam, lparam, &activation);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window, message, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPWSTR command_line, int show) {
    WNDCLASSW window_class = {0};
    window_class.lpfnWndProc = window_procedure;
    window_class.hInstance = instance;
    window_class.lpszClassName = L"wm";
    RegisterClassW(&window_class);
    HWND window = CreateWindowW(L"wm", L"Windows Mobile", WS_VISIBLE, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, instance, NULL);
    SHMENUBARINFO menu_bar = {sizeof menu_bar};
    menu_bar.hwndParent = window;
    menu_bar.dwFlags = SHCMBF_EMPTYBAR;
    SHCreateMenuBar(&menu_bar);
    SHFullScreen(window, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON);
    SHSipPreference(window, SIP_DOWN);
    HANDLE backlight = SetPowerRequirement(L"BKL1:", D0, POWER_NAME, NULL, 0);
    SystemIdleTimerReset();
    SHIdleTimerReset();
    WCHAR state[32];
    DWORD flags = 0;
    GetSystemPowerState(state, 32, &flags);
    PowerPolicyNotify(PPN_UNATTENDEDMODE, TRUE);
    MessageBoxW(window, state, L"Windows Mobile", MB_OK);
    PowerPolicyNotify(PPN_UNATTENDEDMODE, FALSE);
    if (backlight) {
        ReleasePowerRequirement(backlight);
    }
    DestroyWindow(window);
    return 0;
}
