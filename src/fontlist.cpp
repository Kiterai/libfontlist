#include <fontlist.hpp>
// #include <iostream>
#include <stdexcept>
#ifdef _WIN32
#include "font_windows.hpp"
#endif

namespace fontlist {

std::vector<fontfamily> enumerate_font() {
#ifdef _WIN32
    return enumerate_font_win32_dwrite();
#endif
    throw std::runtime_error("fontlist: platform not supported");
}

} // namespace fontlist
