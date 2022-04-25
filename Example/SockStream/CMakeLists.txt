cmake_minimum_required(VERSION 3.16)
project(sockstreamExample)

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

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)

target_link_libraries(${PROJECT_NAME} ${SOCKETOR_LIBRARY})
