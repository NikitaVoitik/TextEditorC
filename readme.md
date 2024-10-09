### install libs
#### Mac os:
    brew install sdl2 sdl2_ttf
#### Windows:
    git clone https://github.com/microsoft/vcpkg.git
    .\vcpkg\bootstrap-vcpkg.bat

    .\vcpkg\vcpkg install sdl2 sdl2-ttf
    
    .\vcpkg\vcpkg integrate install


### build
    mkdir build
    cd build
    cmake ..
    cmake --build .

### run
#### Mac os:
    ./TextEditor
#### Windows:
    TextEditor.exe

