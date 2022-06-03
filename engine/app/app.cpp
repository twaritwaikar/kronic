#include "app.h"

#include "core/log.h"
#include "os/file_system.h"

Application::Application()
{
	FileSystem::set_current_directory_to_root_file("kronic.root");
	INFO("Current directory: {}", FileSystem::get_current_directory());

#ifndef NDEBUG
	Log::enable_debug_logs();
#endif
}
