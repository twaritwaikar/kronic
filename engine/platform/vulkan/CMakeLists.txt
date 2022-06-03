add_library(vulkan-renderer vulkan_renderer.cpp)

target_link_libraries(vulkan-renderer PUBLIC kronic_engine vulkan)
