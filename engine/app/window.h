#pragma once

#include "common.h"

class Window
{
public:
	Window();
	virtual ~Window();

	virtual int32_t get_width() const = 0;
	virtual int32_t get_height() const = 0;
	virtual void set_width(int32_t width) = 0;
	virtual void set_height(int32_t height) = 0;
};
