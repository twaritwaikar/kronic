#include "vulkan_renderer.h"

#include "core/log.h"
#include "core/math.h"
#include "os/file_system.h"
#include "vulkan_init_helpers.h"
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

VulkanRenderer::VulkanRenderer(const char* app_name, const GLFWWindow* window)
{
	build_vulkan_contexts(app_name, window);
	build_swapchain(window->get_width(), window->get_height());
	build_queue_and_command_buffers();
	build_default_render_pass();
	build_framebuffers();
	build_sync_objects();
	build_pipelines();

	is_ok = true;
}

VulkanRenderer::~VulkanRenderer()
{
	if (!is_ok)
	{
		return;
	}

	VK_CHECK(vkWaitForFences(device, 1, &render_fence, true, uint64_t(1) * Convert::s_to_ns));

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
	VK_CHECK(vkWaitForFences(device, 1, &render_fence, true, 1 * Convert::s_to_ns));
	VK_CHECK(vkResetFences(device, 1, &render_fence));

	uint32_t swapchain_image_index = 0;
	VK_CHECK(vkAcquireNextImageKHR(device, swapchain, 1 * Convert::s_to_ns, present_semaphore, nullptr, &swapchain_image_index));

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
			vkCmdBindPipeline(graphics_queue_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, triangle_pipeline);
			vkCmdDraw(graphics_queue_command_buffer, 3, 1, 0, 0);
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

void VulkanRenderer::build_pipelines()
{
	VkShaderModule frag_module;
	if (!load_shader("assets/shaders/shader.frag", ShaderType::Fragment, &frag_module))
	{
		ERR("Could not create fragment shader: {}", "assets/shaders/shader.frag");
	}

	VkShaderModule vert_module;
	if (!load_shader("assets/shaders/shader.vert", ShaderType::Vertex, &vert_module))
	{
		ERR("Could not create vertex shader: {}", "assets/shaders/shader.vert");
	}

	VkPipelineLayoutCreateInfo pipeline_layout_info = VulkanInit::pipeline_layout_create_info();
	VK_CHECK(vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &triangle_pipeline_layout));

	PipelineBuilder pipeline_builder;
	pipeline_builder.shader_stages.push_back(VulkanInit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, vert_module));
	pipeline_builder.shader_stages.push_back(VulkanInit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, frag_module));

	pipeline_builder.vertex_input_info = VulkanInit::pipeline_vertex_input_state_create_info();
	pipeline_builder.input_assembly = VulkanInit::pipeline_input_assembly_state_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	pipeline_builder.viewport.x = 0.0f;
	pipeline_builder.viewport.y = 0.0f;
	pipeline_builder.viewport.width = swapchain_image_width;
	pipeline_builder.viewport.height = swapchain_image_height;
	pipeline_builder.viewport.minDepth = 0.0f;
	pipeline_builder.viewport.maxDepth = 1.0f;

	pipeline_builder.scissor.offset = { 0, 0 };
	pipeline_builder.scissor.extent = { swapchain_image_width, swapchain_image_height };

	pipeline_builder.rasterizer = VulkanInit::pipeline_rasterization_state_create_info(VK_POLYGON_MODE_FILL);

	pipeline_builder.multisampling = VulkanInit::pipeline_multisample_state_create_info();
	pipeline_builder.color_blend_attachment = VulkanInit::color_blend_attachment_state();
	pipeline_builder.pipeline_layout = triangle_pipeline_layout;

	triangle_pipeline = pipeline_builder.build_pipeline(device, render_pass);
}

bool VulkanRenderer::load_shader(const char* file_path, ShaderType type, VkShaderModule* out_shader_module)
{
	Optional<File> file_data = FileSystem::read_file(file_path);
	if (!file_data)
	{
		ERR("Could not load shader from: {}", file_path);
		return {};
	}

	static shaderc::Compiler compiler;
	shaderc::CompilationResult result = compiler.CompileGlslToSpv(file_data->contents, shaderc_shader_kind(int(type)), file_path);

	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		ERR("Found {} errors and {} warnings while compiling shader {}: {}", result.GetNumWarnings(), result.GetNumErrors(), file_path, result.GetErrorMessage());
		return {};
	}

	VkShaderModuleCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.pNext = nullptr;

	create_info.codeSize = sizeof(uint32_t) * (result.end() - result.begin());
	create_info.pCode = result.begin();

	VkShaderModule shader_module;
	if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
	{
		ERR("Could not load shader module from: {}", file_path);
		return {};
	}

	*out_shader_module = shader_module;

	return true;
}

VkPipeline VulkanRenderer::PipelineBuilder::build_pipeline(VkDevice device, VkRenderPass pass)
{
	VkPipelineViewportStateCreateInfo viewport_info {};
	viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_info.pNext = nullptr;

	viewport_info.viewportCount = 1;
	viewport_info.pViewports = &viewport;
	viewport_info.scissorCount = 1;
	viewport_info.pScissors = &scissor;

	VkPipelineColorBlendStateCreateInfo color_blending = {};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.pNext = nullptr;

	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;

	VkGraphicsPipelineCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.pNext = nullptr;

	pipeline_info.stageCount = shader_stages.size();
	pipeline_info.pStages = shader_stages.data();
	pipeline_info.pVertexInputState = &vertex_input_info;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_info;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.layout = pipeline_layout;
	pipeline_info.renderPass = pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

	VkPipeline new_pipeline;
	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &new_pipeline) != VK_SUCCESS)
	{
		ERR("Couldn't create pipeline");
		return VK_NULL_HANDLE;
	}

	return new_pipeline;
}
