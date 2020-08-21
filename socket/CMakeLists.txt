cmake_minimum_required(VERSION 3.15)
project(socket)

set(CMAKE_CXX_STANDARD 17)

add_executable(socket main.cpp Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h)