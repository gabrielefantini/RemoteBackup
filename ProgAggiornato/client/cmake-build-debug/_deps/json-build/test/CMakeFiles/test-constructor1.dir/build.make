# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2020.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2020.3/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug

# Include any dependencies generated for this target.
include _deps/json-build/test/CMakeFiles/test-constructor1.dir/depend.make

# Include the progress variables for this target.
include _deps/json-build/test/CMakeFiles/test-constructor1.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/json-build/test/CMakeFiles/test-constructor1.dir/flags.make

_deps/json-build/test/CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.o: _deps/json-build/test/CMakeFiles/test-constructor1.dir/flags.make
_deps/json-build/test/CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.o: _deps/json-src/test/src/unit-constructor1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/json-build/test/CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.o"
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.o -c /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/src/unit-constructor1.cpp

_deps/json-build/test/CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.i"
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/src/unit-constructor1.cpp > CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.i

_deps/json-build/test/CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.s"
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/src/unit-constructor1.cpp -o CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.s

# Object files for target test-constructor1
test__constructor1_OBJECTS = \
"CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.o"

# External object files for target test-constructor1
test__constructor1_EXTERNAL_OBJECTS = \
"/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test/CMakeFiles/doctest_main.dir/src/unit.cpp.o"

_deps/json-build/test/test-constructor1: _deps/json-build/test/CMakeFiles/test-constructor1.dir/src/unit-constructor1.cpp.o
_deps/json-build/test/test-constructor1: _deps/json-build/test/CMakeFiles/doctest_main.dir/src/unit.cpp.o
_deps/json-build/test/test-constructor1: _deps/json-build/test/CMakeFiles/test-constructor1.dir/build.make
_deps/json-build/test/test-constructor1: _deps/json-build/test/CMakeFiles/test-constructor1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test-constructor1"
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-constructor1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/json-build/test/CMakeFiles/test-constructor1.dir/build: _deps/json-build/test/test-constructor1

.PHONY : _deps/json-build/test/CMakeFiles/test-constructor1.dir/build

_deps/json-build/test/CMakeFiles/test-constructor1.dir/clean:
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && $(CMAKE_COMMAND) -P CMakeFiles/test-constructor1.dir/cmake_clean.cmake
.PHONY : _deps/json-build/test/CMakeFiles/test-constructor1.dir/clean

_deps/json-build/test/CMakeFiles/test-constructor1.dir/depend:
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test/CMakeFiles/test-constructor1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/json-build/test/CMakeFiles/test-constructor1.dir/depend

