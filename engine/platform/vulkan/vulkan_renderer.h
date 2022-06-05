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
	void build_vulkan_contexts(const char* app_name, const GLFWWindow* window);
	void build_swapchain(uint32_t width, uint32_t height);
	void build_queue_and_command_buffers();
	void build_default_render_pass();
	void build_framebuffers();

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
	uint32_t swapchain_image_width;
	uint32_t swapchain_image_height;

	// Queue handles
	VkQueue graphics_queue;
	uint32_t graphics_queue_family;

	// Command pools/buffers
	VkCommandPool graphics_queue_command_pool;
	VkCommandBuffer graphics_queue_command_buffer;

	// Renderpass objects
	VkRenderPass render_pass;
	Vector<VkFramebuffer> framebuffers;
};
