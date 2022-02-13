find_package(Vulkan REQUIRED)

target_include_directories(kronic_engine PRIVATE ${Vulkan_INCLUDE_DIRS})

target_link_libraries(kronic_engine PRIVATE ${Vulkan_LIBRARY})
