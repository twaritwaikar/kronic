#include "konfig.h"

#include "core/log.h"

Konfig::Konfig()
{
	config_file = *FileSystem::read_yaml("assets/konfig.yaml");

	String rendering_api_str = config_file.root["rendering_api"].as<String>();
	String windowing_api_str = config_file.root["windowing_api"].as<String>();

	if (rendering_api_str == "Vulkan")
	{
		rendering_api = RenderingAPI::Vulkan;
	}
	else
	{
		ERR("Found unknown rendering API: {}", rendering_api_str);
	}

	if (windowing_api_str == "GLFW")
	{
		windowing_api = WindowingAPI::GLFW;
	}
	else
	{
		ERR("Found unknown windowing API: {}", windowing_api_str);
	}

	INFO("Loaded konfig file from {}", config_file.path);
}
