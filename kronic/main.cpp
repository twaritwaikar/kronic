#include <stdexcept>

#include "core/log.h"
#include "app.h"

int main()
{
	HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception* e)
	{
		Log::error(e->what());
		return 1;
	}

	return 0;
}
