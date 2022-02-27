add_executable(kronic_tests test.cpp)

add_subdirectory(vendor)

target_link_libraries(kronic_tests PRIVATE kronic_engine)
