#include "vulkan_renderer.h"

#include "vulkan_init_helpers.h"
#include "core/log.h"
#include "platform/glfw/glfw_window.h"

#include "VkBootstrap.h"

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

	graphics_queue_family = vkb_device.get_queue_index(vkb::QueueType::graphics).value();
	graphics_queue = vkb_device.get_queue(vkb::QueueType::graphics).value();

	build_swapchain(window->get_width(), window->get_height());
	build_queue_and_command_buffers();

	is_ok = true;
}

VulkanRenderer::~VulkanRenderer()
{
	if (!is_ok)
	{
		return;
	}

	vkDestroyCommandPool(device, graphics_queue_command_pool, nullptr);
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

void VulkanRenderer::build_queue_and_command_buffers()
{
	VkCommandPoolCreateInfo cmd_pool_info = VulkanInit::command_pool_create_info(graphics_queue_family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	VK_CHECK(vkCreateCommandPool(
	    device,
	    &cmd_pool_info,
	    nullptr,
	    &graphics_queue_command_pool));

	VkCommandBufferAllocateInfo cmd_buffer_info = VulkanInit::command_buffer_allocate_info(graphics_queue_command_pool);
	VK_CHECK(vkAllocateCommandBuffers(
	    device,
	    &cmd_buffer_info,
	    &graphics_queue_command_buffer));
}
