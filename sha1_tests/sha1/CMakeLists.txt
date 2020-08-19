cmake_minimum_required(VERSION 3.15)
project(sha1)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
include_directories(${OpenSSL_INCLUDE_DIRS})
link_libraries(stdc++fs)
link_libraries(crypto)

add_executable(sha1 main.cpp)
target_link_libraries(sha1 ${OpenSSL_LIBRARIES})
