cmake_minimum_required(VERSION 3.16)
project(file_watcher)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(stdc++fs)

add_executable(file_watcher main.cpp FileWatcher.h)
target_link_libraries(file_watcher ${Boost_LIBRARIES})

