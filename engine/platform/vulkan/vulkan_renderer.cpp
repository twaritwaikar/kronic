#include "vulkan_renderer.h"

#include "VkBootstrap.h"
#include "platform/glfw/glfw_window.h"

#define VK_CHECK(x)                      \
	do                                   \
	{                                    \
		VkResult err = x;                \
		if (err)                         \
		{                                \
			CRITICAL("Vulkan: {}", err); \
		}                                \
	} while (0)

VulkanRenderer::VulkanRenderer(const char* app_name, const GLFWWindow* window)
{
	vkb::detail::Result<vkb::Instance> builder_instance
	    = vkb::InstanceBuilder()
	          .set_app_name(app_name)
#ifndef NDEBUG
	          .request_validation_layers(true)
#endif
	          .use_default_debug_messenger()
	          .require_api_version(1, 1, 0)
	          .build();

	vkb::Instance vkb_instance = builder_instance.value();

	instance = vkb_instance.instance;
	debug_messenger = vkb_instance.debug_messenger;
	surface = window->get_surface(instance);

	vkb::PhysicalDevice vkb_physical_device
	    = vkb::PhysicalDeviceSelector { vkb_instance }
	          .set_minimum_version(1, 1)
	          .set_surface(surface)
	          .select()
	          .value();

	vkb::Device vkb_device = vkb::DeviceBuilder { vkb_physical_device }.build().value();

	device = vkb_device.device;
	gpu = vkb_physical_device.physical_device;

	build_swapchain(window->get_width(), window->get_height());

	is_ok = true;
}

VulkanRenderer::~VulkanRenderer()
{
	if (!is_ok)
	{
		return;
	}

	vkDestroySwapchainKHR(device, swapchain, nullptr);

	for (VkImageView& v : swapchain_image_views)
	{
		vkDestroyImageView(device, v, nullptr);
	}

	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkb::destroy_debug_utils_messenger(instance, debug_messenger);
	vkDestroyInstance(instance, nullptr);
}

void VulkanRenderer::build_swapchain(uint32_t width, uint32_t height)
{
	vkb::Swapchain vkb_swapchain
	    = vkb::SwapchainBuilder { gpu, device, surface }
	          .use_default_format_selection()
	          .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
	          .set_desired_extent(width, height)
	          .build()
	          .value();

	swapchain = vkb_swapchain.swapchain;
	swapchain_images = vkb_swapchain.get_images().value();
	swapchain_image_views = vkb_swapchain.get_image_views().value();
	swapchain_image_format = vkb_swapchain.image_format;
}
