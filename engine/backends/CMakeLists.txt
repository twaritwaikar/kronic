add_subdirectory(vulkan)

if(WIN32)
    add_subdirectory(windows)
endif()

if(UNIX AND NOT APPLE)
    add_subdirectory(linux)
endif()

if(APPLE)
    add_subdirectory(mac)
endif()

add_library(backends
    window_glfw.cpp
    window_none.cpp
)

target_link_libraries(backends PUBLIC glfw vulkan_renderer kronic_engine)
