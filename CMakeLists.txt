cmake_minimum_required(VERSION 3.15)

project(fontlist)

add_library(fontlist src/fontlist.cpp src/font_windows.cpp)
target_include_directories(fontlist PUBLIC include)
set_target_properties(fontlist PROPERTIES CXX_STANDARD 17)

if(WIN32)
target_link_libraries(fontlist "Dwrite.lib")
endif()

message(STATUS "libfontlist: build test ${BUILD_TESTS}")
if(BUILD_TESTS)
add_executable(test_enum test/enumerate.cpp)
target_link_libraries(test_enum fontlist)
set_target_properties(test_enum PROPERTIES CXX_STANDARD 17)
endif()
