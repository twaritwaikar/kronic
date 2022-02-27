set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<$<CONFIG:Debug>:Release>$<$<CONFIG:Release>:Debug>/tests/)

add_executable(kronic_tests test.cpp)

add_subdirectory(vendor)

target_link_libraries(kronic_tests PRIVATE kronic_engine)
