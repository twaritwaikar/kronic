#pragma once

#include "vulkan/vulkan.h"

namespace VulkanInit
{
VkCommandPoolCreateInfo command_pool_create_info(uint32_t queue_family, VkCommandPoolCreateFlags flags = 0);
VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
};
