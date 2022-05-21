#include <stdexcept>

#include "core/log.h"
#include "kronic_app.h"

#include "platform/glfw_window.h"

int main()
{
	GLFWWindow glfw_window;

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
