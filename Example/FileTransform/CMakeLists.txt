cmake_minimum_required(VERSION 3.15)
project(FileTransform)

set(CMAKE_CXX_STANDARD 20)
# please change it to you socketor build path
option(SOCKETOR_PATH /usr/local/socketor)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SOCKETOR_PATH})
find_package(socketor)
if(SOCKETOR_FOUND)
    message("Found socketor")
else()
    message("socketor no found")
endif()

include_directories("E:/Github/fast_io/include")

add_executable(receiver receiver.cpp)
add_executable(sender sender.cpp)


target_link_libraries(receiver ${SERVER_LIBRARY})
target_link_libraries(sender ${CLIENT_LIBRARY})
