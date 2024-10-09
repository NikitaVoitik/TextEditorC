cmake_minimum_required(VERSION 3.29)
project(TextEditor C)

set(CMAKE_C_STANDARD 23)

# Include directories for SDL2 and SDL2_ttf
include_directories(/opt/homebrew/opt/sdl2/include/SDL2)
include_directories(/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2)
include_directories(/Users/nikitavoitik/Documents/prog/cpp/TextEditor/libtinyfiledialogs)

# Add the executable with tinyfiledialogs.c
add_executable(TextEditor main.c /Users/nikitavoitik/Documents/prog/cpp/TextEditor/libtinyfiledialogs/tinyfiledialogs.c)

# Link SDL2 and SDL2_ttf libraries
target_link_libraries(TextEditor /opt/homebrew/opt/sdl2/lib/libSDL2.dylib)
target_link_libraries(TextEditor /opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib/libSDL2_ttf-2.0.0.dylib)

# Link necessary frameworks for tinyfiledialogs on macOS
if(APPLE)
    target_link_libraries(TextEditor "-framework Cocoa")
endif()
