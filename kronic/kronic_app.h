#pragma once

#include "app/app.h"
#include "app/konfig.h"
#include "core/log.h"
#include "core/event_link.h"
#include "core/event.h"

class Window;
class Renderer;

class KronicApplication : public Application
{
public:
	KronicApplication();
	~KronicApplication();

	void run() override;

private:
	Konfig konfig;
	Ptr<Window> window;
	// Ptr<Renderer> renderer;

	void handle_resize(const Event::Window::Resizing& e);
	EventLink<KronicApplication, Event::Window::Resizing> event_resize = { this, &KronicApplication::handle_resize };
};
