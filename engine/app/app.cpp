#include "app.h"

#include "app/window.h"

void Application::set_window_renderer(Ref<Window> app_window, Ref<Renderer> app_renderer)
{
	window = app_window;
	renderer = app_renderer;
}

void Application::run()
{
	while (!window->has_closed())
	{
		window->collect_events();
	}
}
