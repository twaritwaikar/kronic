find_package(Vulkan REQUIRED)

message(STATUS "Vulkan SDK: " $ENV{VULKAN_SDK})
message(STATUS "Vulkan include directory: " ${Vulkan_INCLUDE_DIR})

add_library(vulkan ../stub.cpp)

target_include_directories(vulkan PUBLIC ${Vulkan_INCLUDE_DIR})

target_link_directories(vulkan PUBLIC $ENV{VULKAN_SDK}/lib)
target_link_libraries(vulkan PRIVATE
    ${Vulkan_LIBRARY}
    shaderc_combined
)
