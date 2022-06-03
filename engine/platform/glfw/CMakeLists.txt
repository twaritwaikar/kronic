add_library(glfw-window glfw_window.cpp)

target_link_libraries(glfw-window PUBLIC kronic_engine glfw)
