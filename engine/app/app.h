#pragma once

#include "common.h"

class Window;
class Renderer;

class Application
{
public:
	Application() = default;
	virtual ~Application() = default;

	void run();

	void set_window_renderer(Ref<Window> app_window, Ref<Renderer> app_renderer);

private:
	Ref<Window> window = nullptr;
	Ref<Renderer> renderer = nullptr;
};
