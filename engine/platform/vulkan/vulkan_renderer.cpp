#include "vulkan_renderer.h"

#include "vulkan_init_helpers.h"
#include "core/log.h"
#include "platform/glfw/glfw_window.h"

#include "VkBootstrap.h"

#define VK_CHECK(x)                      \
	do                                   \
	{                                    \
		VkResult err = x;                \
		if (err)                         \
		{                                \
			CRITICAL("Vulkan: {}", err); \
		}                                \
	} while (0)

constexpr uint32_t s_to_ns = 1000000000;

VulkanRenderer::VulkanRenderer(const char* app_name, const GLFWWindow* window)
{
	build_vulkan_contexts(app_name, window);
	build_swapchain(window->get_width(), window->get_height());
	build_queue_and_command_buffers();
	build_default_render_pass();
	build_framebuffers();
	build_sync_objects();

	is_ok = true;
}

VulkanRenderer::~VulkanRenderer()
{
	if (!is_ok)
	{
		return;
	}

	VK_CHECK(vkWaitForFences(device, 1, &render_fence, true, 1 * s_to_ns));

	vkDestroyCommandPool(device, graphics_queue_command_pool, nullptr);
	vkDestroySwapchainKHR(device, swapchain, nullptr);

	vkDestroySemaphore(device, present_semaphore, nullptr);
	vkDestroySemaphore(device, render_semaphore, nullptr);
	vkDestroyFence(device, render_fence, nullptr);

	vkDestroyRenderPass(device, render_pass, nullptr);

	for (int i = 0; i < swapchain_image_views.size(); i++)
	{
		vkDestroyFramebuffer(device, framebuffers[i], nullptr);
		vkDestroyImageView(device, swapchain_image_views[i], nullptr);
	}

	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkb::destroy_debug_utils_messenger(instance, debug_messenger);
	vkDestroyInstance(instance, nullptr);
}

void VulkanRenderer::draw()
{
	VK_CHECK(vkWaitForFences(device, 1, &render_fence, true, 1 * s_to_ns));
	VK_CHECK(vkResetFences(device, 1, &render_fence));

	uint32_t swapchain_image_index = 0;
	VK_CHECK(vkAcquireNextImageKHR(device, swapchain, 1 * s_to_ns, present_semaphore, nullptr, &swapchain_image_index));

	VK_CHECK(vkResetCommandBuffer(graphics_queue_command_buffer, 0));
	VkCommandBufferBeginInfo cmd_begin_info = {};
	cmd_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmd_begin_info.pNext = nullptr;

	cmd_begin_info.pInheritanceInfo = nullptr;
	cmd_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_CHECK(vkBeginCommandBuffer(graphics_queue_command_buffer, &cmd_begin_info));
	{
		VkRenderPassBeginInfo render_pass_begin_info = {};
		render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_begin_info.pNext = nullptr;

		render_pass_begin_info.renderPass = render_pass;
		render_pass_begin_info.renderArea.offset.x = 0;
		render_pass_begin_info.renderArea.offset.y = 0;
		render_pass_begin_info.renderArea.extent.width = swapchain_image_width;
		render_pass_begin_info.renderArea.extent.height = swapchain_image_height;
		render_pass_begin_info.framebuffer = framebuffers[swapchain_image_index];

		VkClearValue clear_value = {};
		float flash = std::abs(std::sin(frame_number / 120.0f));
		clear_value.color = { { 0.0f, 0.0f, flash, 1.0f } };

		render_pass_begin_info.clearValueCount = 1;
		render_pass_begin_info.pClearValues = &clear_value;

		vkCmdBeginRenderPass(graphics_queue_command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		{
		}
		vkCmdEndRenderPass(graphics_queue_command_buffer);
	}
	VK_CHECK(vkEndCommandBuffer(graphics_queue_command_buffer));

	VkSubmitInfo submit = {};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.pNext = nullptr;

	VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	submit.pWaitDstStageMask = &wait_stage;
	submit.waitSemaphoreCount = 1;
	submit.pWaitSemaphores = &present_semaphore;
	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &render_semaphore;
	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &graphics_queue_command_buffer;

	VK_CHECK(vkQueueSubmit(graphics_queue, 1, &submit, render_fence));

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.pNext = nullptr;
	present_info.pSwapchains = &swapchain;
	present_info.swapchainCount = 1;
	present_info.pWaitSemaphores = &render_semaphore;
	present_info.waitSemaphoreCount = 1;
	present_info.pImageIndices = &swapchain_image_index;

	VK_CHECK(vkQueuePresentKHR(graphics_queue, &present_info));

	frame_number++;
}

void VulkanRenderer::build_vulkan_contexts(const char* app_name, const GLFWWindow* window)
{
	vkb::detail::Result<vkb::Instance> builder_instance
	    = vkb::InstanceBuilder()
	          .set_app_name(app_name)
#ifndef NDEBUG
	          .request_validation_layers(true)
#endif
	          .use_default_debug_messenger()
	          .require_api_version(1, 1, 0)
	          .build();

	vkb::Instance vkb_instance = builder_instance.value();

	instance = vkb_instance.instance;
	debug_messenger = vkb_instance.debug_messenger;
	surface = window->get_surface(instance);

	vkb::PhysicalDevice vkb_physical_device
	    = vkb::PhysicalDeviceSelector { vkb_instance }
	          .set_minimum_version(1, 1)
	          .set_surface(surface)
	          .select()
	          .value();

	vkb::Device vkb_device = vkb::DeviceBuilder { vkb_physical_device }.build().value();

	device = vkb_device.device;
	gpu = vkb_physical_device.physical_device;

	graphics_queue_family = vkb_device.get_queue_index(vkb::QueueType::graphics).value();
	graphics_queue = vkb_device.get_queue(vkb::QueueType::graphics).value();
}

void VulkanRenderer::build_swapchain(uint32_t width, uint32_t height)
{
	vkb::Swapchain vkb_swapchain
	    = vkb::SwapchainBuilder { gpu, device, surface }
	          .use_default_format_selection()
	          .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
	          .set_desired_extent(width, height)
	          .build()
	          .value();

	swapchain = vkb_swapchain.swapchain;
	swapchain_image_width = width;
	swapchain_image_height = height;
	swapchain_images = vkb_swapchain.get_images().value();
	swapchain_image_views = vkb_swapchain.get_image_views().value();
	swapchain_image_format = vkb_swapchain.image_format;
}

void VulkanRenderer::build_queue_and_command_buffers()
{
	VkCommandPoolCreateInfo cmd_pool_info = VulkanInit::command_pool_create_info(graphics_queue_family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	VK_CHECK(vkCreateCommandPool(
	    device,
	    &cmd_pool_info,
	    nullptr,
	    &graphics_queue_command_pool));

	VkCommandBufferAllocateInfo cmd_buffer_info = VulkanInit::command_buffer_allocate_info(graphics_queue_command_pool);
	VK_CHECK(vkAllocateCommandBuffers(
	    device,
	    &cmd_buffer_info,
	    &graphics_queue_command_buffer));
}

void VulkanRenderer::build_default_render_pass()
{
	// Image lifetime -
	//
	// UNDEFINED
	//   RenderPass Begins
	//     Subpass 0 begins(Transition to Attachment Optimal)
	//     Subpass 0 renders
	//     Subpass 0 ends
	//   Renderpass Ends(Transitions to Present Source)

	VkAttachmentDescription color_attachment = {};
	color_attachment.format = swapchain_image_format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment_ref = {};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;

	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &color_attachment;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;

	VK_CHECK(vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass));
}

void VulkanRenderer::build_framebuffers()
{
	VkFramebufferCreateInfo fb_info = {};
	fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_info.pNext = nullptr;

	fb_info.renderPass = render_pass;
	fb_info.attachmentCount = 1;
	fb_info.width = swapchain_image_width;
	fb_info.height = swapchain_image_height;
	fb_info.layers = 1;

	framebuffers = Vector<VkFramebuffer>(swapchain_images.size());

	for (int i = 0; i < swapchain_images.size(); i++)
	{
		fb_info.pAttachments = &swapchain_image_views[i];
		VK_CHECK(vkCreateFramebuffer(device, &fb_info, nullptr, &framebuffers[i]));
	}
}

void VulkanRenderer::build_sync_objects()
{
	VkFenceCreateInfo fence_create_info = {};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.pNext = nullptr;

	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VK_CHECK(vkCreateFence(device, &fence_create_info, nullptr, &render_fence));

	VkSemaphoreCreateInfo semaphore_create_info = {};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphore_create_info.pNext = nullptr;
	semaphore_create_info.flags = 0;

	VK_CHECK(vkCreateSemaphore(device, &semaphore_create_info, nullptr, &present_semaphore));
	VK_CHECK(vkCreateSemaphore(device, &semaphore_create_info, nullptr, &render_semaphore));
}
