#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "app/window.h"

class WindowGLFW : public Window
{
public:
	WindowGLFW();
	~WindowGLFW();

private:
	GLFWwindow* glfw_window;
};
