cmake_minimum_required(VERSION 3.29)
project(TextEditor C)

set(CMAKE_C_STANDARD 23)

include_directories(/opt/homebrew/opt/sdl2/include/SDL2)
include_directories(/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2)

find_package(SDL2 REQUIRED)
add_executable(TextEditor main.c)

target_link_libraries(TextEditor /opt/homebrew/opt/sdl2/lib/libSDL2.dylib)
target_link_libraries(TextEditor /opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib/libSDL2_ttf-2.0.0.dylib)





