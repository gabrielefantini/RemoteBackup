cmake_minimum_required(VERSION 3.16)
project(Client)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(stdc++fs)


find_package(OpenSSL REQUIRED)
include_directories(${OpenSSL_INCLUDE_DIRS})
link_libraries(stdc++fs)
link_libraries(crypto)



add_executable(Client main.cpp fileWatcher/FileWatcher.h fileWatcher/FileWatcher.cpp hashManager/hashManager.h hashManager/hashManager.cpp)
