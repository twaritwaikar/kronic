set(INSTALL_GTEST OFF)
set(BUILD_GMOCK OFF)

add_subdirectory(googletest)

target_include_directories(kronic_tests PRIVATE
    googletest/googletest/include/
)

target_link_libraries(kronic_tests PRIVATE
    gtest
)
