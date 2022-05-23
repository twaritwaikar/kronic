#pragma once

#include "app/window.h"
#include "backends/vulkan/vulkan_renderer.h"

#include "GLFW/glfw3.h"

class WindowGLFW : public Window
{
public:
	WindowGLFW(int32_t width, int32_t height);
	~WindowGLFW();

	int32_t get_height() const override;
	int32_t get_width() const override;
	void set_height(int32_t height) override;
	void set_width(int32_t width) override;

	bool has_closed() const override;
	void collect_events() const override;

	VkSurfaceKHR get_surface(VulkanRenderer* renderer) const;

private:
	GLFWwindow* glfw_window = nullptr;
};
