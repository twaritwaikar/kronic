add_library(windows os.cpp)

target_link_libraries(windows PUBLIC kronic_engine User32.lib)
