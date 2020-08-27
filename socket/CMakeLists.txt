cmake_minimum_required(VERSION 3.15)
project(socket)

find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

link_libraries(stdc++fs)

set(CMAKE_CXX_STANDARD 17)

add_executable(socket main.cpp Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h)

target_link_libraries(socket ${Boost_LIBRARIES})