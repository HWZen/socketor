cmake_minimum_required(VERSION 3.15)
project(FileTransform)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "E:/Github/socketor/cmake-build-debug-visual-studio")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "E:/Github/socketor/cmake-build-debug-visual-studio")
find_package(socketor)
if(SOCKETOR_FOUND)
    message("Found socketor")
else()
    message("socketor no found")
endif()

include_directories(${SOCKETOR_INCLUDE_DIR})
include_directories("E:/Github/fast_io/include")

add_executable(receiver receiver.cpp)
add_executable(sender sender.cpp)


target_link_libraries(receiver ${SERVER_LIBRARY})
target_link_libraries(sender ${CLIENT_LIBRARY})
