#pragma once

#include "common.h"
#include "os/file_system.h"

struct Konfig
{
	Konfig();

	enum class RenderingAPI
	{
		Vulkan,
		None
	};
	RenderingAPI rendering_api = RenderingAPI::None;

	enum class WindowingAPI
	{
		GLFW,
		None
	};
	WindowingAPI windowing_api = WindowingAPI::None;

	FileYAML config_file;
};
