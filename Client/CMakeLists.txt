cmake_minimum_required(VERSION 3.16)
project(client)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
include_directories(${OpenSSL_INCLUDE_DIRS})
find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

link_libraries(stdc++fs)
link_libraries(crypto)

add_executable(client main.cpp fileWatcher/FileWatcher.h fileWatcher/FileWatcher.cpp hashManager/hashManager.h hashManager/hashManager.cpp)

target_link_libraries(client ${Boost_LIBRARIES} ${OpenSSL_LIBRARIES})
