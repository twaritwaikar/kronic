cmake_minimum_required(VERSION 3.18)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

option(BUILD_TESTS "Build tests" ON)

set(CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 17)

project(
    kronic
    LANGUAGES CXX
    VERSION 1.0
    DESCRIPTION "FPS engine"
)

add_subdirectory(engine)
add_subdirectory(kronic)

if (BUILD_TESTS)
    message(STATUS "Building test suite")
    add_subdirectory(tests)
endif()
