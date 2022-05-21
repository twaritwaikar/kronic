#include <stdexcept>

#include "core/log.h"
#include "kronic_app.h"

#include "platform/window_glfw.h"

int main()
{
	WindowGLFW glfw_window(100, 100);

	KronicApplication app(glfw_window);

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		Log::error(e.what());
		return 1;
	}

	return 0;
}
