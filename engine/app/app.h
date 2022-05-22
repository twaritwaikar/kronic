#pragma once

#include "common.h"

class Window;

class Application
{
public:
	Application(Ref<Window> app_window);
	virtual ~Application();

	void run();

private:
	Ref<Window> window;
};
