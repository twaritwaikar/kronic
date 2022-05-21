add_library(kronic_engine stub.cpp)

target_precompile_headers(kronic_engine PUBLIC common.h)
target_include_directories(kronic_engine PUBLIC ./)

add_subdirectory(app)
add_subdirectory(core)

target_link_libraries(kronic_engine PUBLIC app core)
