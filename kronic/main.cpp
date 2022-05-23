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
	catch (const Exception& e)
	{
		Log::error(e.what());
		return 1;
	}

	return 0;
}
