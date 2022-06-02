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
	void handle_resize(const Event::Window::Resized& e);

	Ptr<Window> window;
	// Ptr<Renderer> renderer;

	Konfig konfig;

	EventLink<KronicApplication, Event::Window::Resized> event_resize = { this, &KronicApplication::handle_resize };
};
