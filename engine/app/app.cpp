#include "app.h"

#include "window.h"
#include "core/log.h"

Application::Application(Window& app_window)
    : window(app_window)
{
	Log::info("Window Size: {}x{}", window.get_width(), window.get_height());
}

Application::~Application()
{
}

void Application::run()
{
}
