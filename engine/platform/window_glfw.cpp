#include "window_glfw.h"

#include "core/log.h"
#include "os/os.h"

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

WindowGLFW::WindowGLFW(uint32_t width, uint32_t height)
{
	static GLFWContext glfw_context;

	glfw_window = glfwCreateWindow(width, height, "Kronic", nullptr, nullptr);
	if (!glfw_window)
	{
		Log::error("Could not create window");
	}
}

WindowGLFW::~WindowGLFW()
{
	glfwDestroyWindow(glfw_window);
}

uint32_t WindowGLFW::get_height() const
{
	int32_t height = 0;
	glfwGetWindowSize(glfw_window, nullptr, &height);

	return height;
}

uint32_t WindowGLFW::get_width() const
{
	int32_t width = 0;
	glfwGetWindowSize(glfw_window, &width, nullptr);

	return width;
}

void WindowGLFW::set_width(uint32_t width)
{
	glfwSetWindowSize(glfw_window, width, get_height());
}

void WindowGLFW::set_height(uint32_t height)
{
	glfwSetWindowSize(glfw_window, get_width(), height);
}

bool WindowGLFW::has_closed() const
{
	return glfwWindowShouldClose(glfw_window);
}

void WindowGLFW::collect_events() const
{
	glfwPollEvents();
}
