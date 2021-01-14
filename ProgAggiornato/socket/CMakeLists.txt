cmake_minimum_required(VERSION 3.15)
project(socket)

find_package(OpenSSL REQUIRED)
include_directories(${OpenSSL_INCLUDE_DIRS})

find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

link_libraries(stdc++fs)
link_libraries(crypto)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_STANDARD 17)

add_executable(socket main.cpp Socket.cpp Socket.h ServerSocket.cpp ServerSocket.h jsonUtils.cpp jsonUtils.h serverUtils/serverUtils.cpp serverUtils/serverUtils.h hashManager/hashManager.h hashManager/hashManager.cpp)

target_link_libraries(socket ${Boost_LIBRARIES} nlohmann_json::nlohmann_json)