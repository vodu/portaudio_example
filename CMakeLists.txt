cmake_minimum_required(VERSION 3.20)

set(VCPKG_TARGET_TRIPLET x64-windows)
set(VCPKG_BUILD_TYPE release)

project(pa_example)

find_package(portaudio CONFIG REQUIRED)

add_executable(pa_example main.cpp)
target_link_libraries(pa_example PRIVATE portaudio)
