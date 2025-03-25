#include <fontlist.hpp>
#include <iostream>
#include <windows.h>

int main() {
    try {
        std::cout << "System fonts enumerating..." << std::endl;

        auto fontfamilies = fontlist::enumerate_font();

        for (const auto &fontfamily : fontfamilies) {
            std::cout << fontfamily.name << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
