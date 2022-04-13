cmake_minimum_required(VERSION 3.15)
project(socketor)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_library(sockclient STATIC sock_client.cpp socketor.cpp)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#target_link_libraries(sockclient -lwsock32)
endif()

add_library(sockserver STATIC sock_server.cpp socketor.cpp)

file(COPY include DESTINATION ./)
file(COPY cmake/Findsocketor.cmake DESTINATION ./)