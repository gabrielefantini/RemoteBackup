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
include _deps/json-build/test/CMakeFiles/test-merge_patch.dir/depend.make

# Include the progress variables for this target.
include _deps/json-build/test/CMakeFiles/test-merge_patch.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/json-build/test/CMakeFiles/test-merge_patch.dir/flags.make

_deps/json-build/test/CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.o: _deps/json-build/test/CMakeFiles/test-merge_patch.dir/flags.make
_deps/json-build/test/CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.o: _deps/json-src/test/src/unit-merge_patch.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/json-build/test/CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.o"
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.o -c /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/src/unit-merge_patch.cpp

_deps/json-build/test/CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.i"
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/src/unit-merge_patch.cpp > CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.i

_deps/json-build/test/CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.s"
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test/src/unit-merge_patch.cpp -o CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.s

# Object files for target test-merge_patch
test__merge_patch_OBJECTS = \
"CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.o"

# External object files for target test-merge_patch
test__merge_patch_EXTERNAL_OBJECTS = \
"/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test/CMakeFiles/doctest_main.dir/src/unit.cpp.o"

_deps/json-build/test/test-merge_patch: _deps/json-build/test/CMakeFiles/test-merge_patch.dir/src/unit-merge_patch.cpp.o
_deps/json-build/test/test-merge_patch: _deps/json-build/test/CMakeFiles/doctest_main.dir/src/unit.cpp.o
_deps/json-build/test/test-merge_patch: _deps/json-build/test/CMakeFiles/test-merge_patch.dir/build.make
_deps/json-build/test/test-merge_patch: _deps/json-build/test/CMakeFiles/test-merge_patch.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test-merge_patch"
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-merge_patch.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/json-build/test/CMakeFiles/test-merge_patch.dir/build: _deps/json-build/test/test-merge_patch

.PHONY : _deps/json-build/test/CMakeFiles/test-merge_patch.dir/build

_deps/json-build/test/CMakeFiles/test-merge_patch.dir/clean:
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test && $(CMAKE_COMMAND) -P CMakeFiles/test-merge_patch.dir/cmake_clean.cmake
.PHONY : _deps/json-build/test/CMakeFiles/test-merge_patch.dir/clean

_deps/json-build/test/CMakeFiles/test-merge_patch.dir/depend:
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src/test /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/test/CMakeFiles/test-merge_patch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/json-build/test/CMakeFiles/test-merge_patch.dir/depend

