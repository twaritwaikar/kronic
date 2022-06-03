#pragma once

#include "window.h"

class WindowNone : public Window
{
public:
	WindowNone() = default;
	~WindowNone() = default;

	uint32_t get_height() const override { return height; };
	uint32_t get_width() const override { return width; };
	void set_height(uint32_t new_height) override { height = new_height; };
	void set_width(uint32_t new_width) override { width = new_width; };

	bool has_closed() const override { return is_closing; }
	void collect_events() const override { }

private:
	uint32_t width = 0;
	uint32_t height = 0;
	bool is_closing = false;
};
