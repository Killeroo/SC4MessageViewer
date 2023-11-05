#pragma once

#include <string>
#include <map>

class cConfig
{
public:
	bool CreateDefault(const std::wstring& sPath);
	bool Load(const std::wstring& sPath);

	const std::map<uint32_t, std::string>& GetMessageIds() const { return mMessageIds; }

private:
	std::map<uint32_t, std::string> mMessageIds;
};