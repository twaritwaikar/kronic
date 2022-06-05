#include "vulkan_init_helpers.h"

VkCommandPoolCreateInfo VulkanInit::command_pool_create_info(uint32_t queue_family, VkCommandPoolCreateFlags flags)
{
	VkCommandPoolCreateInfo command_pool_info = {};
	command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_info.pNext = nullptr;

	command_pool_info.queueFamilyIndex = queue_family;
	command_pool_info.flags = flags;
	return command_pool_info;
}

VkCommandBufferAllocateInfo VulkanInit::command_buffer_allocate_info(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level)
{
	VkCommandBufferAllocateInfo cmd_alloc_info = {};
	cmd_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd_alloc_info.pNext = nullptr;

	cmd_alloc_info.commandPool = pool;
	cmd_alloc_info.commandBufferCount = count;
	cmd_alloc_info.level = level;
	return cmd_alloc_info;
}
