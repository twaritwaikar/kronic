#include "kronic_app.h"

#include "core/log.h"
#include "app/window.h"
#include "platform/window_glfw.h"
#include "os/file_system.h"

KronicApplication::KronicApplication()
{
	if (konfig.windowing_api == Konfig::WindowingAPI::GLFW)
	{
		window = MakeUnique<WindowGLFW>(640, 800);
		Log::info("Created a {}x{} GLFW window", window->get_width(), window->get_height());
	}
}

KronicApplication::~KronicApplication()
{
	Log::info("Kronic application is ending");
}

void KronicApplication::run()
{
	while (!window->has_closed())
	{
		window->collect_events();
	}
}
