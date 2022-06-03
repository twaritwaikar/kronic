#pragma once

#include "core/renderer.h"

class WindowGLFW;

class VulkanRenderer : public Renderer
{
public:
	VulkanRenderer(const WindowGLFW* window);
	~VulkanRenderer();
};
