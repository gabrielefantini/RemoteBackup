# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /home/mattia/Scaricati/CLion-2019.3.5/clion-2019.3.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/mattia/Scaricati/CLion-2019.3.5/clion-2019.3.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mattia/Scrivania/client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mattia/Scrivania/client/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/main.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mattia/Scrivania/client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/main.cpp.o -c /home/mattia/Scrivania/client/main.cpp

CMakeFiles/client.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mattia/Scrivania/client/main.cpp > CMakeFiles/client.dir/main.cpp.i

CMakeFiles/client.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mattia/Scrivania/client/main.cpp -o CMakeFiles/client.dir/main.cpp.s

CMakeFiles/client.dir/ClientSocket.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/ClientSocket.cpp.o: ../ClientSocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mattia/Scrivania/client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client.dir/ClientSocket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/ClientSocket.cpp.o -c /home/mattia/Scrivania/client/ClientSocket.cpp

CMakeFiles/client.dir/ClientSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/ClientSocket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mattia/Scrivania/client/ClientSocket.cpp > CMakeFiles/client.dir/ClientSocket.cpp.i

CMakeFiles/client.dir/ClientSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/ClientSocket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mattia/Scrivania/client/ClientSocket.cpp -o CMakeFiles/client.dir/ClientSocket.cpp.s

CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.o: /home/mattia/Scrivania/socket/Socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mattia/Scrivania/client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.o -c /home/mattia/Scrivania/socket/Socket.cpp

CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mattia/Scrivania/socket/Socket.cpp > CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.i

CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mattia/Scrivania/socket/Socket.cpp -o CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/main.cpp.o" \
"CMakeFiles/client.dir/ClientSocket.cpp.o" \
"CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/main.cpp.o
client: CMakeFiles/client.dir/ClientSocket.cpp.o
client: CMakeFiles/client.dir/home/mattia/Scrivania/socket/Socket.cpp.o
client: CMakeFiles/client.dir/build.make
client: /usr/lib/x86_64-linux-gnu/libboost_system.so
client: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mattia/Scrivania/client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/mattia/Scrivania/client/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mattia/Scrivania/client /home/mattia/Scrivania/client /home/mattia/Scrivania/client/cmake-build-debug /home/mattia/Scrivania/client/cmake-build-debug /home/mattia/Scrivania/client/cmake-build-debug/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

