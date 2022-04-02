cmake_minimum_required(VERSION 3.16)
project(test)

set(CMAKE_CXX_STANDARD 17)
# please change it to you socketor build path
option(SOCKETOR_PATH /usr/local/socketor)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SOCKETOR_PATH})
find_package(socketor)
if(SOCKETOR_FOUND)
    message("Found socketor")
else()
    message("socketor no found")
endif()



add_executable(servertest server_test.cpp)
add_executable(clienttest client_test.cpp)


target_link_libraries(servertest ${SERVER_LIBRARY})
target_link_libraries(clienttest ${CLIENT_LIBRARY})



