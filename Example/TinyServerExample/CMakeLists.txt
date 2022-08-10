add_executable(TinyServerExample TinyServerExample.cpp)

target_link_libraries(TinyServerExample socketor)
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(TinyServerExample pthread)
endif()