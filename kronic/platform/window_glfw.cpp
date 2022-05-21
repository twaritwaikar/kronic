#include "window_glfw.h"

#include "core/log.h"

class GLFWContext
{
public:
	static void error_callback(int error, const char* description)
	{
		Log::error("GLFW: %s\n", description);
	}

	GLFWContext()
	{
		if (!glfwInit())
		{
			Log::error("GLFW: Could not initialize");
		}

		glfwSetErrorCallback(error_callback);
	}

	~GLFWContext()
	{
		glfwTerminate();
	}
};

WindowGLFW::WindowGLFW()
{
	static GLFWContext glfw_context;

	glfw_window = glfwCreateWindow(650, 480, "Kronic", nullptr, nullptr);
	if (!glfw_window)
	{
		Log::error("Could not create window");
	}
}

WindowGLFW::~WindowGLFW()
{
	glfwTerminate();
}
