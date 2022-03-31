cmake_minimum_required(VERSION 3.23)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "E:/Github/socketor/cmake-build-debug-visual-studio")
find_package(socketor)
if(SOCKETOR_FOUND)
    message("Found socketor")
else()
    message("socketor no found")
endif()

include_directories(${SOCKETOR_INCLUDE_DIR})

add_executable(servertest server_test.cpp)
add_executable(clienttest client_test.cpp)


target_link_libraries(servertest ${SERVER_LIBRARY})
target_link_libraries(clienttest ${CLIENT_LIBRARY})



