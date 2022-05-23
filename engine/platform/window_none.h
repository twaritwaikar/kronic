#pragma once

#include "app/window.h"

class WindowNone : public Window
{
public:
	WindowNone() = default;
	~WindowNone() = default;

	int32_t get_height() const override { return height; };
	int32_t get_width() const override { return width; };
	void set_height(int32_t new_height) override { height = new_height; };
	void set_width(int32_t new_width) override { width = new_width; };

	bool has_closed() const override { return is_closing; }
	void collect_events() const override { }

private:
	int32_t width = 0;
	int32_t height = 0;
	bool is_closing = false;
};
