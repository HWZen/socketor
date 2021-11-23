cmake_minimum_required(VERSION 3.15)
project(socketor)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(E:\\Github\\rapidjson)
include_directories(include)

add_library(sockclient SHARED sock_client.cpp socketor.cpp)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
target_link_libraries(sockclient -lwsock32)
endif()
target_compile_definitions(sockclient PRIVATE DEC)

add_library(sockserver SHARED sock_server.cpp socketor.cpp)
target_compile_definitions(sockserver PRIVATE DEC)

add_executable(server
        server_test.cpp
        function.cpp
        turing.cpp
        )
target_link_libraries(server PRIVATE
        sockserver
        winhttp
        )


add_executable(client
        client_test.cpp
        function.cpp
        )
target_link_libraries(client PRIVATE sockclient)

add_executable(test function.cpp test.cpp)

