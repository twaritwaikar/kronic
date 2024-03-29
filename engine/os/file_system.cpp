#include "file_system.h"

#include <filesystem>
#include <fstream>

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
		ERR("Could not load YAML file from {}. {}", path, e->what());
	}
	return {};
}

Optional<File> FileSystem::read_file(const String& path)
{
	std::ifstream file(path.c_str(), std::ios::binary);

	if (!file.is_open())
	{
		ERR("Could not read file: {}", path);
		return {};
	}

	File file_obj;
	file_obj.path = path;
	file_obj.contents = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	return file_obj;
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
		ERR("Could not set current directory to {}. {}", root_file_name, e->what());
	}
}

String FileSystem::get_current_directory()
{
	return std::filesystem::current_path().generic_string();
}
