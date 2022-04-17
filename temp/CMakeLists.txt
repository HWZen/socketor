cmake_minimum_required(VERSION 3.15)
project(temp)

set(CMAKE_CXX_STANDARD 20)

option(SOCKETOR_PATH /usr/local/socketor)
include_directories(../include)

include_directories(../../fast_io/include)

add_executable(temp temp.cpp ../src/server.cpp ../src/client.cpp ../src/socketor.cpp)
