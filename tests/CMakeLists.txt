add_executable(kronic_tests
    main.cpp
)

set(vendored_libs "" CACHE INTERNAL "")
set(vendored_includes "" CACHE INTERNAL "")

add_subdirectory(vendor)

target_include_directories(kronic_tests PRIVATE
    ../
    vendor/googletest/googletest/googletest/include/
)

target_link_libraries(kronic_tests
    gtest
    kronic_engine
)
