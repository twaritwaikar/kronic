#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "app/window.h"

class GLFWWindow : public Window
{
public:
	GLFWWindow(uint32_t width, uint32_t height);
	~GLFWWindow();

	uint32_t get_height() const override;
	uint32_t get_width() const override;
	void set_height(uint32_t height) override;
	void set_width(uint32_t width) override;

	bool has_closed() const override;
	void collect_events() const override;

	VkSurfaceKHR get_surface(VkInstance& instance) const;

private:
	GLFWwindow* glfw_window = nullptr;
};
