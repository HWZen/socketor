set(CMAKE_CXX_STANDARD 20)
include_directories("fast_io/include")

add_executable(receiver receiver.cpp)
add_executable(sender sender.cpp)


target_link_libraries(receiver socketor)
target_link_libraries(sender socketor)
