if(WIN32)
    add_subdirectory(windows)
endif()

if(UNIX AND NOT APPLE)
    add_subdirectory(linux)
endif()

if(APPLE)
    add_subdirectory(mac)
endif()

add_library(platform glfw_window.cpp)

target_link_libraries(platform PUBLIC glfw)
