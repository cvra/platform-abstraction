cmake_minimum_required(VERSION 2.6)

project(platform-abstraction)
set(CMAKE_BUILD_TYPE Debug)

add_executable(
    tests
    tests/main.cpp
    tests/platform_mock_test.cpp  
    mock/semaphores.c
    mock/mutex.c
    )

target_link_libraries(
    tests
    CppUTest
    CppUTestExt
    )
