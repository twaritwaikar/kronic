add_library(kronic_engine
    core/engine.cpp
    core/log.cpp
    core/math.cpp
)

target_include_directories(kronic_engine PUBLIC ./)

add_subdirectory(vendor)
