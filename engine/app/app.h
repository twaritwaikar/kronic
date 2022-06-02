#pragma once

#include "common.h"

class Application
{
public:
	Application();
	virtual ~Application() = default;

	virtual void run() = 0;
};
