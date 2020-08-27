cmake_minimum_required(VERSION 3.16)
project(server)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
include_directories(${OpenSSL_INCLUDE_DIRS})
find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

link_libraries(stdc++fs)
link_libraries(crypto)

add_executable(server main.cpp hashManager/hashManager.h hashManager/hashManager.cpp main_util/mainUtilities.cpp main_util/mainUtilities.h socketUtil/Socket.cpp socketUtil/Socket.h socketUtil/ServerSocket.cpp socketUtil/ServerSocket.h)

target_link_libraries(server ${Boost_LIBRARIES} ${OpenSSL_LIBRARIES})