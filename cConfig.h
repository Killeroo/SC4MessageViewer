#pragma once

#include <string>
#include <map>

class cConfig
{

public:
	bool Load(const std::wstring& sPath);

private:
	std::map<uint32_t, std::string> mMessageIds;
};