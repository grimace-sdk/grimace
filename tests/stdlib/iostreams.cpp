#include <windows.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    std::ostringstream formatted;
    formatted << "value " << 42 << ' ' << 2.5 << std::hex << ' ' << 255;
    std::istringstream parsed("17 3.25 word");
    int number = 0;
    double fraction = 0;
    std::string word;
    parsed >> number >> fraction >> word;
    {
        std::ofstream output("\\iostreams-test.txt");
        output << "line one\n" << 7 << '\n';
    }
    std::ifstream input("\\iostreams-test.txt");
    std::string first_line;
    int seven = 0;
    std::getline(input, first_line);
    input >> seven;
    std::cout << formatted.str() << std::endl;
    std::cerr << "cerr works" << std::endl;
    std::wostringstream wide;
    wide << L"wide " << 3;
    bool passed = formatted.str() == "value 42 2.5 ff" && number == 17 && fraction == 3.25 && word == "word" &&
                  first_line == "line one" && seven == 7 && wide.str() == L"wide 3";
    MessageBoxW(nullptr, passed ? L"passed" : L"failed", L"iostreams", MB_OK);
    return passed ? 0 : 1;
}
