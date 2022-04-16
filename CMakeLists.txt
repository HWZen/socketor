cmake_minimum_required(VERSION 3.15)
project(socketor)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(sockclient STATIC src/client.cpp src/socketor.cpp src/sockstream.cpp)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#target_link_libraries(sockclient -lwsock32)
endif()

add_library(sockserver STATIC src/server.cpp src/socketor.cpp src/sockstream.cpp)

file(COPY include DESTINATION ./)
file(COPY cmake/Findsocketor.cmake DESTINATION ./)

add_subdirectory(google_test)