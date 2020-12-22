cmake_minimum_required(VERSION 3.15)
project(client)

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

set(CMAKE_CXX_STANDARD 17)


add_executable(client main.cpp ClientSocket.cpp ClientSocket.h ../socket/Socket.cpp fileWatcher/FileWatcher.h fileWatcher/FileWatcher.cpp hashManager/hashManager.h hashManager/hashManager.cpp jsonUtils.cpp jsonUtils.h)
target_link_libraries( client ${Boost_LIBRARIES} ${OpenSSL_LIBRARIES} nlohmann_json::nlohmann_json)
