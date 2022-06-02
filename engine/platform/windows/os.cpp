#include "os/os.h"

#include <Windows.h>

class WindowsOS : public OS
{
public:
	void post_error_message(const String& title, const String& error_msg) override
	{
		MessageBox(GetActiveWindow(), error_msg.c_str(), title.c_str(), MB_CANCELTRYCONTINUE | MB_ICONHAND | MB_DEFBUTTON2);
	}
};

Ptr<OS> OS::singleton = MakeUnique<WindowsOS>();
