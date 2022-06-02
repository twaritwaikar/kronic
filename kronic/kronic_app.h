#pragma once

#include "app/app.h"
#include "app/konfig.h"

class Window;
class Renderer;

class KronicApplication : public Application
{
	Konfig konfig;

public:
	KronicApplication();
	~KronicApplication();

	void run() override;

private:
	Ptr<Window> window;
	// Ptr<Renderer> renderer;
};
