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
	EventWindowClosing::fire({});
}

void KronicApplication::run()
{
	EventWindowResizing event = { {}, 100, 100 };
	while (!window->has_closed())
	{
		renderer->draw();

		window->collect_events();
	}
}

void KronicApplication::handle_resize(const EventWindowResizing& e)
{
	DEBUG("New size: {}x{}", e.width, e.height);
}
