#include "app.h"

#include <set>

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data)
{
	static const char* message_type_string[3] = {
		"General",
		"Validation",
		"Performance"
	};

	const char* current_message_type
	    = (message_type == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)  ? message_type_string[0]
	    : message_type == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT ? message_type_string[1]
	                                                                     : message_type_string[2];

	switch (message_severity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		Log::warn("Vulkan: {}: {}", current_message_type, callback_data->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		Log::error("Vulkan: {}: {}", current_message_type, callback_data->pMessage);
		break;
	default:
	    // Log::info("Vulkan: {}: {}", current_message_type, callback_data->pMessage);
	    ;
	}

	return VK_FALSE;
}

void HelloTriangleApplication::_init_window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::_add_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info)
{
	create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	create_info.pfnUserCallback = debug_callback;
}

void HelloTriangleApplication::_create_instance()
{
	if (enable_validation_layers && !_check_validation_layer_support())
	{
		throw std::runtime_error("Validation layers requested, but not available");
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
	CHECK_VULKAN(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr));

	std::vector<VkExtensionProperties> extensions(extension_count);
	CHECK_VULKAN(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()));

	std::vector<const char*> glfw_extensions = _get_glfw_required_extensions();

	for (auto& glfw_extension_name : glfw_extensions)
	{
		bool found = false;
		for (auto& extension : extensions)
		{
			if (strcmp(extension.extensionName, glfw_extension_name) == 0)
			{
				found = true;

				Log::info("Vulkan: Found support for extension required by GLFW: {}", glfw_extension_name);
				break;
			}
		}

		if (!found)
		{
			Log::warn("Vulkan: Extension required by GLFW not found: {}", glfw_extension_name);
		}
	}

	create_info.enabledExtensionCount = glfw_extensions.size();
	create_info.ppEnabledExtensionNames = glfw_extensions.data();

	create_info.enabledLayerCount = 0;

	VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};
	if (enable_validation_layers)
	{
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();

		_add_debug_messenger_create_info(debug_create_info);
		create_info.pNext = &debug_create_info;
	}
	else
	{
		create_info.enabledLayerCount = 0;
		create_info.pNext = nullptr;
	}

	CHECK_VULKAN(vkCreateInstance(&create_info, nullptr, &instance));
}

std::vector<const char*> HelloTriangleApplication::_get_glfw_required_extensions()
{
	uint32_t glfw_extension_count = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

	if (enable_validation_layers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool HelloTriangleApplication::_check_validation_layer_support()
{
	uint32_t layer_count = 0;
	CHECK_VULKAN(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));

	std::vector<VkLayerProperties> available_layers(layer_count);
	CHECK_VULKAN(vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()));

	for (const char* layer_name : validation_layers)
	{
		bool layer_found = false;

		for (const auto& layer_properties : available_layers)
		{
			if (strcmp(layer_name, layer_properties.layerName) == 0)
			{
				layer_found = true;
				break;
			}
		}

		if (!layer_found)
		{
			Log::error("Vulkan: Could not find layer support: {}", layer_name);
			return false;
		}
	}

	return true;
}

void HelloTriangleApplication::_setup_debug_messenger()
{
	if (!enable_validation_layers)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT create_info;
	_add_debug_messenger_create_info(create_info);

	CHECK_VULKAN_EXT(vkCreateDebugUtilsMessengerEXT, instance, &create_info, nullptr, &debug_messenger);
}

void HelloTriangleApplication::_init_vulkan()
{
	_create_instance();
	_setup_debug_messenger();
	_create_surface();
	_pick_physical_device();
	_create_logical_device();
}

void HelloTriangleApplication::_main_loop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
}

void HelloTriangleApplication::_clean_up()
{
	if (enable_validation_layers)
	{
		VULKAN_EXT(vkDestroyDebugUtilsMessengerEXT, instance, debug_messenger, nullptr);
	}

	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void HelloTriangleApplication::_pick_physical_device()
{
	uint32_t device_count = 0;
	CHECK_VULKAN(vkEnumeratePhysicalDevices(instance, &device_count, nullptr));
	if (!device_count)
	{
		throw std::runtime_error("Failed to find a GPU with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(device_count);
	CHECK_VULKAN(vkEnumeratePhysicalDevices(instance, &device_count, devices.data()));

	uint32_t current_device_score = 0;

	for (const auto& device : devices)
	{
		uint32_t score = _calculate_device_score(device);
		if (score > current_device_score)
		{
			physical_device = device;
			current_device_score = score;
		}
	}

	if (physical_device == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU");
	}
}

uint32_t HelloTriangleApplication::_calculate_device_score(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties(device, &device_properties);

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(device, &device_features);

	QueueFamilyIndices indices = _find_queue_families(device);

	// We only accept GPUs which support our required Vulkan queue families
	if (!indices.is_complete())
	{
		return 0;
	}

	// Application can't function without geometry shaders
	if (!device_features.geometryShader)
	{
		return 0;
	}

	uint32_t score = 0;

	// Discrete GPUs have a major performance advantage over non-discrete GPUs
	score += 1000 * static_cast<uint32_t>(device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);

	// Maximum possible size of textures affects graphics quality
	score += static_cast<uint32_t>(device_properties.limits.maxImageDimension2D);

	return score;
}

HelloTriangleApplication::QueueFamilyIndices HelloTriangleApplication::_find_queue_families(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

	int i = 0;
	for (const auto& queue_family : queue_families)
	{
		if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphics_family = i;
		}

		VkBool32 present_support = false;
		CHECK_VULKAN(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support));
		if (present_support)
		{
			indices.present_family = i;
		}

		if (indices.is_complete())
		{
			break;
		}

		i++;
	}

	return indices;
}

void HelloTriangleApplication::_create_logical_device()
{
	QueueFamilyIndices indices = _find_queue_families(physical_device);

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos {};
	std::set<uint32_t> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

	float queue_priority = 1.0f;
	for (uint32_t queue_family : unique_queue_families)
	{
		VkDeviceQueueCreateInfo queue_create_info {};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queue_family;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;
		queue_create_infos.push_back(queue_create_info);
	}

	VkDeviceCreateInfo create_info {};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());

	VkPhysicalDeviceFeatures device_features {};
	create_info.pEnabledFeatures = &device_features;

	create_info.enabledExtensionCount = 0;

	if (enable_validation_layers)
	{
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();
	}
	else
	{
		create_info.enabledLayerCount = 0;
	}

	CHECK_VULKAN(vkCreateDevice(physical_device, &create_info, nullptr, &device));

	vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &graphics_queue);
	vkGetDeviceQueue(device, indices.present_family.value(), 0, &present_queue);
}

void HelloTriangleApplication::_create_surface()
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface");
	}
}
