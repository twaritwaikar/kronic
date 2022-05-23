add_library(vulkan_renderer
    vulkan_renderer.cpp
)

target_link_libraries(vulkan_renderer PUBLIC vulkan vk-bootstrap::vk-bootstrap kronic_engine)
