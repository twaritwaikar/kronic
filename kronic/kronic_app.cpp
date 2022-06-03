#include "kronic_app.h"

#include "core/log.h"
#include "platform/vulkan/vulkan_renderer.h"
#include "platform/glfw/glfw_window.h"

KronicApplication::KronicApplication()
{
	if (konfig.windowing_api == Konfig::WindowingAPI::GLFW)
	{
		window = MakeUnique<GLFWWindow>(640, 480); // TODO: Should be in user konfig
		INFO("Created a {}x{} GLFW window", window->get_width(), window->get_height());
	}

	if (konfig.rendering_api == Konfig::RenderingAPI::Vulkan)
	{
		renderer = MakeUnique<VulkanRenderer>("Kronic", static_cast<GLFWWindow*>(window.get())); // TODO: Should be in user konfig
		INFO("Created Vulkan renderer");
	}
}

KronicApplication::~KronicApplication()
{
	INFO("Kronic application is ending");
	Event::Window::Closing::propagate({});
}

void KronicApplication::run()
{
	Event::Window::Resizing event = { {}, 100, 100 };
	while (!window->has_closed())
	{
		Event::Window::Resizing::propagate(event);

		window->collect_events();
	}
}

void KronicApplication::handle_resize(const Event::Window::Resizing& e)
{
	DEBUG("New size: {}x{}", e.width, e.height);
}
