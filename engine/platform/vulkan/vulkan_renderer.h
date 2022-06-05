#pragma once

#include "core/renderer.h"

#include "vulkan/vulkan.h"

class GLFWWindow;

class VulkanRenderer : public Renderer
{
public:
	VulkanRenderer(const char* app_name, const GLFWWindow* window);
	~VulkanRenderer();

private:
	void build_swapchain(uint32_t width, uint32_t height);
	void build_queue_and_command_buffers();

	// Context variables
	bool is_ok = false;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	VkPhysicalDevice gpu;
	VkDevice device;
	VkSurfaceKHR surface;

	// Swapchain
	VkSwapchainKHR swapchain;
	VkFormat swapchain_image_format;
	Vector<VkImage> swapchain_images;
	Vector<VkImageView> swapchain_image_views;

	// Queue handles
	VkQueue graphics_queue;
	uint32_t graphics_queue_family;
	VkCommandPool graphics_queue_command_pool;
	VkCommandBuffer graphics_queue_command_buffer;
};
