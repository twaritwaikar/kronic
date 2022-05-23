#include "vulkan_renderer.h"

#include "backends/window_glfw.h"

#include "VkBootstrap.h"

VulkanRenderer::VulkanRenderer()
{
	vkb::InstanceBuilder builder;
	vkb::detail::Result<vkb::Instance> instance_returned
	    = builder.set_app_name("Kronic")
#ifdef DEBUG
	          .request_validation_layers(true)
#endif
	          .require_api_version(1, 1, 0)
	          .use_default_debug_messenger()
	          .build();
	vkb::Instance vkb_instance = instance_returned.value();

	instance = vkb_instance.instance;
	debug_messenger = vkb_instance.debug_messenger;
}

VulkanRenderer::~VulkanRenderer()
{
	vkDestroySurfaceKHR(instance, surface, nullptr);
}

void VulkanRenderer::link_to_window(WindowGLFW* window)
{
	surface = window->get_surface(this);
}
