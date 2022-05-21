add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(vulkan)

add_library(vendor stub.cpp)

target_include_directories(vendor PUBLIC 
    glfw/glfw/include/
    glm/glm/
    spdlog/spdlog/include/
)

target_link_libraries(vendor glfw spdlog::spdlog vulkan)
