#include "common.h"

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
		Log::error(e.what());
		return 1;
	}

	return 0;
}
