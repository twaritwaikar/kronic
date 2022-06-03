#pragma once

#include "common.h"

class Window
{
public:
	Window() = default;
	virtual ~Window() = default;

	virtual uint32_t get_width() const = 0;
	virtual uint32_t get_height() const = 0;
	virtual void set_width(uint32_t width) = 0;
	virtual void set_height(uint32_t height) = 0;

	virtual bool has_closed() const = 0;

	virtual void collect_events() const = 0;
};
