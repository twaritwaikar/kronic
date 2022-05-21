add_library(kronic_engine
    app.cpp
    engine.cpp
)

target_precompile_headers(kronic_engine PUBLIC common.h)
target_include_directories(kronic_engine PUBLIC ./)

add_subdirectory(core)
add_subdirectory(vendor)

target_link_libraries(kronic_engine PUBLIC core vendor)
