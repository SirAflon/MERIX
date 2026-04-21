#pragma once
#include <iostream>


namespace output{
    enum Color {
        FG_RED = 31,
        FG_GREEN = 32,
        FG_BLUE = 34,
        FG_DEFAULT = 39,
        FG_BLACK = 30,
        FG_YELLOW = 33,
        FG_MAGENTA = 35,
        FG_CYAN = 36,
        FG_LIGHT_GRAY = 37,
        FG_DARK_GRAY = 90,
        FG_LIGHT_RED = 91,
        FG_LIGHT_GREEN = 92,
        FG_LIGHT_YELLOW = 93,
        FG_LIGHT_BLUE = 94,
        FG_LIGHT_MAGENTA = 95,
        FG_LIGHT_CYAN = 96,
        FG_WHITE = 97,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49
    };
    class UseColor {
        Color code;
    public:
        UseColor(Color pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const UseColor& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
    template<typename... Args>
    void print(Args... args) {
        (std::cout << ... << args);
    }
    template<typename... Args>
    void println(Args... args) {
        (std::cout << ... << args) << '\n';
    }
    inline const UseColor reset(FG_DEFAULT);
    inline const UseColor red(FG_RED);
    inline const UseColor green(FG_GREEN);
}