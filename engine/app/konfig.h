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
	} rendering_api = RenderingAPI::None;

	enum class WindowingAPI
	{
		GLFW,
		None
	} windowing_api = WindowingAPI::None;

	FileYAML config_file;
};
