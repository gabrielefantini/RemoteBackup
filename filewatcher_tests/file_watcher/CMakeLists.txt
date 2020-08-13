cmake_minimum_required(VERSION 3.16)
project(file_watcher)

set(CMAKE_CXX_STANDARD 17)

link_libraries(stdc++fs)

add_executable(file_watcher main.cpp FileWatcher.h)