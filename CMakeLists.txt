cmake_minimum_required(VERSION 3.20)

project(pa_example)

find_package(portaudio CONFIG REQUIRED)

add_executable(pa_example main.cpp)
target_link_libraries(pa_example PRIVATE portaudio)
