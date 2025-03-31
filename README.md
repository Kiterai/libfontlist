# libfontlist

C++17 cross-platform library for retrieving information about system-registered font families. This library supports Windows, Linux, and macOS.

## Supported Backends

- Windows: [DirectWrite](https://learn.microsoft.com/en-us/windows/win32/directwrite/direct-write-portal)
- Linux: [fontconfig](https://www.freedesktop.org/wiki/Software/fontconfig/)
- macOS: [CoreText](https://developer.apple.com/documentation/coretext)

## Requirement

- On Linux, the `libfontconfig-dev` package is required to build.

## Example

```cmake
# CMakeLists.txt
add_executable(fonts_enum main.cpp)
target_link_libraries(fonts_enum fontlist)
set_target_properties(fonts_enum PROPERTIES CXX_STANDARD 17)
```

```cpp
// Source
#include <fontlist.hpp>
#include <iostream>

int main() {
    try {
        std::cout << "System fonts enumerating..." << std::endl;

        auto fontfamilies = fontlist::enumerate_font();

        for (const auto &fontfamily : fontfamilies) {
            std::cout << fontfamily.name << std::endl;
            for (const auto &font : fontfamily.fonts) {
                std::cout << "  " << font.file << " weight:" << font.weight << " style:" << fontlist::to_string(font.style) << std::endl;
            }
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
```

## Important Notes

- `fontlist::font::weight` is intended to correspond to the CSS `font-weight`, but it may not match perfectly, especially on macOS. This value may not be a multiple of 100.
