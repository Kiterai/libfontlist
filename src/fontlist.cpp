#include <fontlist.hpp>
// #include <iostream>
#include <stdexcept>
#ifdef _WIN32
#include "font_windows.hpp"
#endif
#ifdef __linux__
#include "font_linux.hpp"
#endif

namespace fontlist {

std::vector<fontfamily> enumerate_font() {
#ifdef _WIN32
    return enumerate_font_win32_dwrite();
#endif
#ifdef __linux__
    return enumerate_font_linux_fontconfig();
#endif
    throw std::runtime_error("fontlist: platform not supported");
}

} // namespace fontlist
