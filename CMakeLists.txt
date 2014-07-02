cmake_minimum_required(VERSION 2.6)

project(platform-abstraction)
set(CMAKE_BUILD_TYPE Debug)

add_executable(
    tests
    tests/main.cpp
    tests/semaphore_mock_test.cpp
    tests/mutex_mock_test.cpp
    tests/criticalsection_mock_test.cpp
    tests/panic_mock_test.cpp
    tests/malloc_test.cpp
    xmalloc.c
    panic.c
    mock/semaphores.c
    mock/mutex.c
    )

target_link_libraries(
    tests
    CppUTest
    CppUTestExt
    )
