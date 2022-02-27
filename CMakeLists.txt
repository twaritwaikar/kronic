cmake_minimum_required(VERSION 3.18)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

option(BUILD_TESTS "Build tests" OFF)

set(CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 17)

set(BUILD_CONFIG $<$<CONFIG:Debug>:Release>$<$<CONFIG:Release>:Debug>)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${BUILD_CONFIG}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${BUILD_CONFIG}/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${BUILD_CONFIG}/)

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
