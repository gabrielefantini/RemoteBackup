cmake_minimum_required(VERSION 3.15)
project(client)

find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

link_libraries(stdc++fs)

set(CMAKE_CXX_STANDARD 17)

add_executable(client main.cpp ClientSocket.cpp ClientSocket.h ../socket/Socket.cpp)

target_link_libraries(client ${Boost_LIBRARIES})