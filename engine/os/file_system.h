#pragma once

#include "common.h"

#include "yaml-cpp/yaml.h"

struct File
{
	String path;
};

struct FileYAML : public File
{
	YAML::Node root;
};

struct FileSystem
{
	static Optional<FileYAML> read_yaml(const String& path);

	static void set_current_directory_to_root_file(const String& root_file_name);
	static String get_current_directory();
};
