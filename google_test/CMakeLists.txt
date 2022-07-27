# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
find_package(GTest CONFIG REQUIRED)


# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run socketTest.cpp ../src/socketor.cpp ../src/client.cpp ../src/server.cpp serverTest.cpp clientTest.cpp connectTest.cpp)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(Google_Tests_run gtest gtest_main)
    target_link_libraries(Google_Tests_run -lpthread)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(Google_Tests_run -static GTest::gtest GTest::gtest_main)
endif()

