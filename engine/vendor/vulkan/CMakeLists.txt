find_package(Vulkan REQUIRED)

target_include_directories(kronic_engine PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_directories(kronic_engine PUBLIC $ENV{VULKAN_SDK}/lib)
target_link_libraries(kronic_engine PRIVATE
    ${Vulkan_LIBRARY}
    shaderc_combined
)
