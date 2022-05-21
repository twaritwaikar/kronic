if(WIN32)
    add_subdirectory(windows)
endif()

if(UNIX AND NOT APPLE)
    add_subdirectory(linux)
endif()

if(APPLE)
    add_subdirectory(mac)
endif()

add_library(platform
    window_glfw.cpp
    window_none.cpp
)

target_link_libraries(platform PUBLIC glfw kronic_engine)
