# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.4/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nikitavoitik/Documents/prog/cpp/TextEditor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nikitavoitik/Documents/prog/cpp/TextEditor/build

# Include any dependencies generated for this target.
include CMakeFiles/TextEditor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TextEditor.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TextEditor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TextEditor.dir/flags.make

CMakeFiles/TextEditor.dir/main.c.o: CMakeFiles/TextEditor.dir/flags.make
CMakeFiles/TextEditor.dir/main.c.o: /Users/nikitavoitik/Documents/prog/cpp/TextEditor/main.c
CMakeFiles/TextEditor.dir/main.c.o: CMakeFiles/TextEditor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/nikitavoitik/Documents/prog/cpp/TextEditor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/TextEditor.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/TextEditor.dir/main.c.o -MF CMakeFiles/TextEditor.dir/main.c.o.d -o CMakeFiles/TextEditor.dir/main.c.o -c /Users/nikitavoitik/Documents/prog/cpp/TextEditor/main.c

CMakeFiles/TextEditor.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/TextEditor.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/nikitavoitik/Documents/prog/cpp/TextEditor/main.c > CMakeFiles/TextEditor.dir/main.c.i

CMakeFiles/TextEditor.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/TextEditor.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/nikitavoitik/Documents/prog/cpp/TextEditor/main.c -o CMakeFiles/TextEditor.dir/main.c.s

# Object files for target TextEditor
TextEditor_OBJECTS = \
"CMakeFiles/TextEditor.dir/main.c.o"

# External object files for target TextEditor
TextEditor_EXTERNAL_OBJECTS =

TextEditor: CMakeFiles/TextEditor.dir/main.c.o
TextEditor: CMakeFiles/TextEditor.dir/build.make
TextEditor: /opt/homebrew/opt/sdl2/lib/libSDL2.dylib
TextEditor: CMakeFiles/TextEditor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/nikitavoitik/Documents/prog/cpp/TextEditor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable TextEditor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TextEditor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TextEditor.dir/build: TextEditor
.PHONY : CMakeFiles/TextEditor.dir/build

CMakeFiles/TextEditor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TextEditor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TextEditor.dir/clean

CMakeFiles/TextEditor.dir/depend:
	cd /Users/nikitavoitik/Documents/prog/cpp/TextEditor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nikitavoitik/Documents/prog/cpp/TextEditor /Users/nikitavoitik/Documents/prog/cpp/TextEditor /Users/nikitavoitik/Documents/prog/cpp/TextEditor/build /Users/nikitavoitik/Documents/prog/cpp/TextEditor/build /Users/nikitavoitik/Documents/prog/cpp/TextEditor/build/CMakeFiles/TextEditor.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TextEditor.dir/depend

