#pragma once

#include "common.h"

class Renderer
{
public:
	Renderer() = default;
	virtual ~Renderer() = default;

	virtual const char* get_api_name() const = 0;
};
