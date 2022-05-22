#include <stdexcept>

#include "common.h"
#include "core/log.h"
#include "platform/window_glfw.h"

#include "kronic_app.h"

int main()
{
	Ref<WindowGLFW> glfw_window = MakeRef<WindowGLFW>(100, 100);
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
