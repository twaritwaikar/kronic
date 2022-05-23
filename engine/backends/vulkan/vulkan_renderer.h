#pragma once

#include "core/renderer/renderer.h"

#include "vulkan_common.h"

class WindowGLFW;

class VulkanRenderer : public Renderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	const char* get_api_name() const { return "Vulkan"; }
	const VkInstance& get_instance() const { return instance; }

	void link_to_window(WindowGLFW* window);

private:
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	VkPhysicalDevice physical_device;
	VkDevice device;
	VkSurfaceKHR surface;
};
