#pragma once

#include <vector>

#include "core/log.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

const std::vector<const char*> validation_layers = {
	"VK_LAYER_KHRONOS_VALIDATION"
};

const bool enable_validation_layers
#ifdef NDEBUG
    = false;
#else
    = true;
#endif

class HelloTriangleApplication
{
public:
	void run()
	{
		_init_window();
		_init_vulkan();
		_main_loop();
		_clean_up();
	}

private:
	GLFWwindow* window = nullptr;

	VkInstance instance;

	void _init_window()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void _create_instance()
	{
		if (enable_validation_layers && !check_validation_layer_support())
		{
			throw std::runtime_error("Validation layers requestion, but not available");
		}

		VkApplicationInfo app_info {};

		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName = "Hello Triangle";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName = "kronic";
		app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo create_info {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;

		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

		for (auto& extension : extensions)
		{
			Log::info("Found support for extension: {}@{}", extension.extensionName, extension.specVersion);
		}

		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		int32_t i = -1;
		while (++i != glfw_extension_count)
		{
			const char* glfw_extension_name = glfw_extensions[i];

			bool found = false;
			for (auto& extension : extensions)
			{
				if (extension.extensionName == glfw_extension_name)
				{
					found = true;

					Log::info("Found support for extension required by GLFW: {}", glfw_extension_name);
					break;
				}
			}

			if (!found)
			{
				Log::warn("Vulkan extension required by GLFW not found: {}", glfw_extension_name);
			}
		}

		create_info.enabledExtensionCount = glfw_extension_count;
		create_info.ppEnabledExtensionNames = glfw_extensions;
		create_info.enabledLayerCount = 0;

		if (enable_validation_layers)
		{
			create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
			create_info.ppEnabledLayerNames = validation_layers.data();
		}

		if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
		{
			Log::critical("Could not create VKInstance");
			throw std::runtime_error("Failed to create VkInstance");
		}
	}

	bool check_validation_layer_support()
	{
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> available_layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

		for (const char* layer_name : validation_layers)
		{
			bool layer_found = false;

			for (const auto& layer_properties : available_layers)
			{
				if (!strcmp(layer_name, layer_properties.layerName))
				{
					layer_found = true;
					break;
				}
			}

			if (!layer_found)
			{
				return false;
			}
		}
	}

	void _init_vulkan()
	{
		_create_instance();
	}

	void _main_loop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void _clean_up()
	{
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}
};
