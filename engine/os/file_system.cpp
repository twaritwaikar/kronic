#include "file_system.h"

#include <filesystem>

#include "core/log.h"

Optional<FileYAML> FileSystem::read_yaml(const String& path)
{
	try
	{
		YAML::Node node = YAML::LoadFile(path);

		if (node)
		{
			FileYAML yaml_file;
			yaml_file.path = path;
			yaml_file.root = node;

			return yaml_file;
		}
	}
	catch (const Exception* e)
	{
		Log::error("Could not load YAML file from {}. {}", path, e->what());
	}
	return {};
}

void FileSystem::set_current_directory_to_root_file(const String& root_file_name)
{
	try
	{
		std::filesystem::path path = std::filesystem::current_path();
		while (true)
		{
			if (std::filesystem::exists(path / root_file_name))
			{
				break;
			}
			path = path.parent_path();
		}

		std::filesystem::current_path(path);
	}
	catch (const Exception* e)
	{
		Log::error("Could not set current directory to {}. {}", root_file_name, e->what());
	}
}

String FileSystem::get_current_directory()
{
	return std::filesystem::current_path().generic_string();
}
