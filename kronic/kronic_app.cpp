#include "kronic_app.h"

#include "backends/window_glfw.h"
#include "backends/vulkan/vulkan_renderer.h"

KronicApplication::KronicApplication()
{
	Ref<WindowGLFW> glfw_window = MakeRef<WindowGLFW>(100, 100);

	Ref<VulkanRenderer> vulkan_renderer = MakeRef<VulkanRenderer>();
	vulkan_renderer->link_to_window(glfw_window.get());

	set_window_renderer(glfw_window, vulkan_renderer);
}

KronicApplication::~KronicApplication()
{
}
