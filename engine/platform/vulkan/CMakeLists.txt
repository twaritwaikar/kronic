add_library(vulkan-renderer vulkan_renderer.cpp "vulkan_init_helpers.h" "vulkan_init_helpers.cpp")

target_link_libraries(vulkan-renderer PUBLIC kronic_engine vulkan vk-bootstrap)
