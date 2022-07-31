cmake_minimum_required(VERSION 3.15)
project(socketor)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_library(socketor STATIC src/client.cpp src/server.cpp src/socketor.cpp)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#target_link_libraries(sockclient -lwsock32)
endif()

file(COPY include DESTINATION ./)
file(COPY cmake/Findsocketor.cmake DESTINATION ./)

option(GTEST "Enable Google Test" OFF)
if(GTEST)
    add_subdirectory(google_test)
endif()

option(EXAMPLE "Build examples" OFF)
if(EXAMPLE)
    add_subdirectory(Example)
endif()