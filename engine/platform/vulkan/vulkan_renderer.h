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

	bool is_ok = false;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	VkPhysicalDevice gpu;
	VkDevice device;
	VkSurfaceKHR surface;

	VkSwapchainKHR swapchain;
	VkFormat swapchain_image_format;
	Vector<VkImage> swapchain_images;
	Vector<VkImageView> swapchain_image_views;
};
