add_library(linux os.cpp)

target_link_libraries(linux PUBLIC kronic_engine User32.lib)
