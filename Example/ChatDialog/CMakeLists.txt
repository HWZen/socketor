cmake_minimum_required(VERSION 3.16)
project(ChatDialog)

set(CMAKE_CXX_STANDARD 17)

option(QT_PATH "Qt root path" OFF)
if (QT_PATH STREQUAL "OFF")
    message(STATUS "No set QT_PATH")
else()
    message(STATUS "QT_PATH: ${QT_PATH}")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_PATH})
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    set(QT_VERSION 5)
    set(REQUIRED_LIBS Core Gui Widgets OpenGL)
    set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)

    if(CMAKE_VERSION VERSION_LESS "3.7.0")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
    endif()

    find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

    add_executable(${PROJECT_NAME} welcome.ui mainwindow.ui mainwindow.cpp welcome.cpp main.cpp ServerThread.cpp ClientThread.cpp)

    target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
    target_link_libraries(${PROJECT_NAME} socketor)

endif ()

