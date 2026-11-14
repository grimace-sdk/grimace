#include <windows.h>

#include <stdexcept>
#include <string>
#include <typeinfo>

struct shape {
    virtual ~shape() {}
};

struct circle : shape {
    int radius = 3;
};

struct named {
    virtual ~named() {}
};

struct labelled_circle : circle, named {};

struct failure : std::runtime_error {
    failure() : std::runtime_error("failure") {}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    labelled_circle labelled;
    shape *as_shape = &labelled;
    named *as_named = &labelled;
    failure error;
    std::exception *as_exception = &error;
    int score = 0;
    score += dynamic_cast<circle *>(as_shape) != nullptr;
    score += dynamic_cast<named *>(as_shape) == as_named;
    score += dynamic_cast<failure *>(as_exception) == &error;
    score += dynamic_cast<std::logic_error *>(as_exception) == nullptr;
    score += typeid(*as_shape) == typeid(labelled_circle);
    score += std::string(typeid(error).name()).size() > 0;
    std::wstring text = std::to_wstring(score);
    MessageBoxW(nullptr, text.c_str(), L"rtti", MB_OK);
    return score == 6 ? 0 : 1;
}
