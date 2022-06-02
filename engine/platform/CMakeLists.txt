set(os_library_name "")

if(WIN32)
    add_subdirectory(windows)
    set(os_library_name windows)
endif()

if(UNIX AND NOT APPLE)
    add_subdirectory(linux)
    set(os_library_name linux)
endif()

if(APPLE)
    add_subdirectory(mac)
    set(os_library_name mac)
endif()

add_library(platform "window_glfw.cpp" "window_none.cpp")

target_link_libraries(platform PUBLIC glfw vulkan kronic_engine ${os_library_name})
