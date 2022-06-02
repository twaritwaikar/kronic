#include "kronic_app.h"

#include "core/log.h"
#include "platform/window_glfw.h"

KronicApplication::KronicApplication()
{
	if (konfig.windowing_api == Konfig::WindowingAPI::GLFW)
	{
		window = MakeUnique<WindowGLFW>(640, 480); // Windows dimensions should technically be in user konfigs
		Log::info("Created a {}x{} GLFW window", window->get_width(), window->get_height());
	}
}

KronicApplication::~KronicApplication()
{
	Log::info("Kronic application is ending");
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
	Log::debug("New size: {}x{}", e.width, e.height);
}
