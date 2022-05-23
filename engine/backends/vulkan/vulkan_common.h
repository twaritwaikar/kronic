#pragma once

#include "core/log/log.h"

#include "vulkan/vulkan.h"

#define VK_CHECK(x)                        \
	do                                     \
	{                                      \
		VkResult err = x;                  \
		if (err)                           \
		{                                  \
			Log::error("Vulkan: {}", err); \
			abort();                       \
		}                                  \
	} while (0)
