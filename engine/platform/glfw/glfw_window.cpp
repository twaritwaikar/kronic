#include "glfw_window.h"

#include "core/log.h"
#include "os/os.h"

class GLFWContext
{
public:
	static void error_callback(int error, const char* description)
	{
		ERR("GLFW: %s\n", description);
	}

	GLFWContext()
	{
		if (!glfwInit())
		{
			ERR("GLFW: Could not initialize");
		}

		glfwSetErrorCallback(error_callback);
	}

	~GLFWContext()
	{
		glfwTerminate();
	}
};

GLFWWindow::GLFWWindow(uint32_t width, uint32_t height)
{
	static GLFWContext glfw_context;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfw_window = glfwCreateWindow(width, height, "Kronic", nullptr, nullptr);
	if (!glfw_window)
	{
		ERR("Could not create window");
	}
}

GLFWWindow::~GLFWWindow()
{
	glfwDestroyWindow(glfw_window);
}

uint32_t GLFWWindow::get_height() const
{
	int32_t height = 0;
	glfwGetWindowSize(glfw_window, nullptr, &height);

	return height;
}

uint32_t GLFWWindow::get_width() const
{
	int32_t width = 0;
	glfwGetWindowSize(glfw_window, &width, nullptr);

	return width;
}

void GLFWWindow::set_width(uint32_t width)
{
	glfwSetWindowSize(glfw_window, width, get_height());
}

void GLFWWindow::set_height(uint32_t height)
{
	glfwSetWindowSize(glfw_window, get_width(), height);
}

bool GLFWWindow::has_closed() const
{
	return glfwWindowShouldClose(glfw_window);
}

void GLFWWindow::collect_events() const
{
	glfwPollEvents();
}

VkSurfaceKHR GLFWWindow::get_surface(VkInstance& instance) const
{
	VkSurfaceKHR surface;
	VkResult err = glfwCreateWindowSurface(instance, glfw_window, nullptr, &surface);
	if (err)
	{
		CRITICAL("Could not create Vulkan surface");
	}
	return surface;
}
