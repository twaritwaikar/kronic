#pragma once

#include <vector>
#include <optional>

#include "core/log.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#ifdef NDEBUG
#define CHECK_VULKAN(func)
#else
#define CHECK_VULKAN(func)                               \
	do                                                   \
	{                                                    \
		if (func != VK_SUCCESS)                          \
		{                                                \
			Log::error("Vulkan call failed: {}", #func); \
		}                                                \
	} while (false)
#endif

#define CHECK_VULKAN_EXT(func, ...)                                             \
	do                                                                          \
	{                                                                           \
		if (auto function = (PFN_##func)vkGetInstanceProcAddr(instance, #func)) \
		{                                                                       \
			CHECK_VULKAN(function(__VA_ARGS__));                                \
		}                                                                       \
		else                                                                    \
		{                                                                       \
			throw std::runtime_error("Failed to set up debug messenger");       \
		}                                                                       \
	} while (false)

#define VULKAN_EXT(func, ...)                                                   \
	do                                                                          \
	{                                                                           \
		if (auto function = (PFN_##func)vkGetInstanceProcAddr(instance, #func)) \
		{                                                                       \
			function(__VA_ARGS__);                                              \
		}                                                                       \
		else                                                                    \
		{                                                                       \
			throw std::runtime_error("Failed to set up debug messenger");       \
		}                                                                       \
	} while (false)

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

const std::vector<const char*> validation_layers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> device_extensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
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
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphics_family;
		std::optional<uint32_t> present_family;

		bool is_complete() { return graphics_family.has_value() && present_family.has_value(); }
	};

	enum class ShaderType
	{
		Vertex,
		Fragment,
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> present_modes;
	};

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
	VkDebugUtilsMessengerEXT debug_messenger;
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphics_queue;
	VkQueue present_queue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swap_chain;
	std::vector<VkImage> swap_chain_images;
	VkFormat swap_chain_image_format;
	VkExtent2D swap_chain_extent;
	std::vector<VkImageView> swap_chain_image_views;
	VkRenderPass render_pass;
	VkPipelineLayout pipeline_layout;
	VkPipeline graphics_pipeline;
	std::vector<VkFramebuffer> swap_chain_frame_buffers;
	VkCommandPool command_pool;
	std::vector<VkCommandBuffer> command_buffers;
	VkSemaphore image_available_semaphore;
	VkSemaphore render_finished_semaphore;

	void _init_window();
	void _add_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info);
	void _create_instance();
	std::vector<const char*> _get_glfw_required_extensions();
	bool _check_validation_layer_support();
	void _setup_debug_messenger();
	void _init_vulkan();
	void _main_loop();
	void _clean_up();
	void _pick_physical_device();
	uint32_t _calculate_device_score(VkPhysicalDevice device);
	QueueFamilyIndices _find_queue_families(VkPhysicalDevice device);
	void _create_logical_device();
	void _create_surface();
	bool _check_device_extension_support(VkPhysicalDevice device);
	SwapChainSupportDetails _query_swap_chain_support(VkPhysicalDevice device);
	VkSurfaceFormatKHR _choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
	VkPresentModeKHR _choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes);
	VkExtent2D _choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);
	void _create_swap_chain();
	void _create_image_views();
	void _create_graphics_pipeline();
	VkShaderModule _compile_shader(const char* path, ShaderType type);
	void _create_render_pass();
	void _create_frame_buffers();
	void _create_command_pool();
	void _create_command_buffers();
	void _create_semaphores();
	void _draw_frame();
};
