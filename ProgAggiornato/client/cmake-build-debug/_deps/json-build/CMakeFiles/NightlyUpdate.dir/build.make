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

# Utility rule file for NightlyUpdate.

# Include the progress variables for this target.
include _deps/json-build/CMakeFiles/NightlyUpdate.dir/progress.make

_deps/json-build/CMakeFiles/NightlyUpdate:
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build && /opt/clion-2020.3/bin/cmake/linux/bin/ctest -D NightlyUpdate

NightlyUpdate: _deps/json-build/CMakeFiles/NightlyUpdate
NightlyUpdate: _deps/json-build/CMakeFiles/NightlyUpdate.dir/build.make

.PHONY : NightlyUpdate

# Rule to build all files generated by this target.
_deps/json-build/CMakeFiles/NightlyUpdate.dir/build: NightlyUpdate

.PHONY : _deps/json-build/CMakeFiles/NightlyUpdate.dir/build

_deps/json-build/CMakeFiles/NightlyUpdate.dir/clean:
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build && $(CMAKE_COMMAND) -P CMakeFiles/NightlyUpdate.dir/cmake_clean.cmake
.PHONY : _deps/json-build/CMakeFiles/NightlyUpdate.dir/clean

_deps/json-build/CMakeFiles/NightlyUpdate.dir/depend:
	cd /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-src /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build /home/gabriele/Scrivania/RemoteBackup/ProgAggiornato/client/cmake-build-debug/_deps/json-build/CMakeFiles/NightlyUpdate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/json-build/CMakeFiles/NightlyUpdate.dir/depend

