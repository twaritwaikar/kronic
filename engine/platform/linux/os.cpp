#include "os/os.h"

class GNULinuxOS : public OS
{
public:
	void post_error_message(const String& title, const String& error_msg) override
	{
		// std::system(zenity --error --text='ohno vulkan ded'");
		std::system("echo \\a");
	}
};

Ptr<OS> OS::singleton = MakeUnique<GNULinuxOS>();
