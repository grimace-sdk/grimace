#include <windows.h>

#include <string>

extern char order[16];
extern volatile int order_length;
void record(char step);

__attribute__((constructor(200))) static void second_priority() {
    record('2');
}

__attribute__((constructor(101))) static void first_priority() {
    record('1');
}

struct recorder {
    recorder(char step) {
        record(step);
    }
};

static recorder plain('a');
std::string greeting = "constructed";

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    bool passed = order_length == 4 && order[0] == '1' && order[1] == '2' && greeting == "constructed";
    order[order_length] = 0;
    wchar_t text[32];
    wsprintfW(text, L"%hs %s", order, passed ? L"passed" : L"failed");
    MessageBoxW(nullptr, text, L"constructors", MB_OK);
    return passed ? 0 : 1;
}
