#pragma once

class Window;

class Application
{
public:
	Application(Window& app_window);
	virtual ~Application();

	void run();

private:
	Window& window;
};
