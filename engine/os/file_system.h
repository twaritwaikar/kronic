#pragma once

#include "common.h"

#include "yaml-cpp/yaml.h"

struct BaseFile
{
	String path;
};

struct File : public BaseFile
{
	String contents;
};

struct FileYAML : public BaseFile
{
	YAML::Node root;
};

struct FileSystem
{
	static Optional<FileYAML> read_yaml(const String& path);
	static Optional<File> read_file(const String& path);

	static void set_current_directory_to_root_file(const String& root_file_name);
	static String get_current_directory();
};
