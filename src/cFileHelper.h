#pragma once

#include <filesystem>

class cFileHelper
{
public:
	static std::filesystem::path GetCurrentModuleDirectory();
	static bool DoesFileExist(const std::filesystem::path& path);
};