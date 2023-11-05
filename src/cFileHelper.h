#pragma once

#include <string>

class cFileHelper
{
public:
	static std::wstring GetCurrentPath();
	static std::wstring GetParentDirectory(std::wstring path);
	static bool DoesFileExist(const std::wstring& path);
};