add_executable(servertest server_test.cpp)
add_executable(clienttest client_test.cpp)

target_link_libraries(servertest socketor)
target_link_libraries(clienttest socketor)



