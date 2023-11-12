#pragma once

#include <string>
#include <map>

#include "cLogger.h"

class cConfig
{
public:
	bool CreateDefault(const std::wstring& sPath);
	bool Load(const std::wstring& sPath);

public:
	const std::vector<std::string>& GetSkippedMessages() const { return vSkippedMessages; }
	const std::map<uint32_t, std::string>& GetMessageIds() const { return mMessageIds; }
	const bool LogToConsole() const { return bLogToConsole; }
	const bool LogToFile() const { return bLogToFile; }

private:
	void StringToLower(std::string* string);

private:
	std::vector<std::string> vSkippedMessages;
	std::map<uint32_t, std::string> mMessageIds;
	bool bLogToConsole = true;
	bool bLogToFile = false;
};