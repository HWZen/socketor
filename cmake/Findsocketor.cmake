message("finding socketor")

FIND_PATH(SOCKETOR_INCLUDE_DIR socketor.h ${CMAKE_MODULE_PATH}/include)
message("include dir: ${SOCKETOR_INCLUDE_DIR}")

FIND_LIBRARY(CLIENT_LIBRARY sockclient.lib ${CMAKE_MODULE_PATH}/)
message("client lib dir: ${CLIENT_LIBRARY}")

FIND_LIBRARY(SERVER_LIBRARY sockserver.lib ${CMAKE_MODULE_PATH}/)
message("server lib dir: ${SERVER_LIBRARY}")

if(SOCKETOR_INCLUDE_DIR AND CLIENT_LIBRARY AND SERVER_LIBRARY)
    set(SOCKETOR_FOUND TRUE)
endif(SOCKETOR_INCLUDE_DIR AND CLIENT_LIBRARY AND SERVER_LIBRARY)