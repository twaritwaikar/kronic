find_package(Vulkan REQUIRED)

message(STATUS "Vulkan SDK: " $ENV{VULKAN_SDK})
message(STATUS "Vulkan include directory: " ${Vulkan_INCLUDE_DIR})

target_include_directories(kronic_engine PUBLIC ${Vulkan_INCLUDE_DIR})

target_link_directories(kronic_engine PUBLIC $ENV{VULKAN_SDK}/lib)
target_link_libraries(kronic_engine PRIVATE
    ${Vulkan_LIBRARY}
    shaderc_combined
)
