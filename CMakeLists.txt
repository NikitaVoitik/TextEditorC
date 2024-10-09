cmake_minimum_required(VERSION 3.29)
project(TextEditor C)

set(CMAKE_C_STANDARD 23)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(libtinyfiledialogs)

add_executable(TextEditor main.c libtinyfiledialogs/tinyfiledialogs.c)

target_link_libraries(TextEditor SDL2::SDL2 SDL2_ttf::SDL2_ttf)