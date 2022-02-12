cmake_minimum_required(VERSION 3.18)

option(TESTS "Build tests" OFF)

set(CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 20)

project(
    kronic
    LANGUAGES CXX
    VERSION 1.0
    DESCRIPTION "FPS engine"
)

add_subdirectory(src)
add_subdirectory(engine)
add_subdirectory(kronic)

if (TESTS)
    add_subdirectory(tests)
endif()
