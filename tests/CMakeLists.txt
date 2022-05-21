add_executable(kronic_tests test.cpp)

target_link_libraries(kronic_tests PUBLIC kronic_engine gtest)
