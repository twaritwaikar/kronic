#include "common.h"
#include "core/log.h"

#include "kronic_app.h"

int main()
{
	Ref<KronicApplication> app = MakeRef<KronicApplication>();

	try
	{
		app->run();
	}
	catch (const Exception& e)
	{
		Log::critical(e.what());
		return 1;
	}

	return 0;
}
