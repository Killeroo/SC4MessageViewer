#include "cConfig.h"

#include <sstream>
#include <fstream>
#include <vector>

#include "cConsoleLogger.h"

bool cConfig::Load(const std::wstring& sPath)
{
	std::ifstream fileStream(sPath.c_str());
	if (fileStream.fail())
	{
		cConsoleLogger::LogMessage(eConsoleColor::RED, L"Could not find config file");
		return false;
	}

	std::string line;
	std::vector<std::string> skippedProperties;
	while (std::getline(fileStream, line))
	{
		// Ignore comment lines
		if (line.at(1) == ';')
		{
			continue;
		}

		std::size_t separatorPos = line.find('=');
		if (separatorPos != std::string::npos)
		{
			// Check if there is a comment at the end of the line
			const std::size_t commentPos = line.find(';');

			// Extract propery and it's value
			const std::string property = line.substr(0, separatorPos);
			const std::size_t valueLen = (commentPos == std::string::npos ? line.size() : commentPos + 1) - separatorPos;
			const std::string value = line.substr(separatorPos + 1, valueLen);

			printf("%s:%s\n", property.c_str(), value.c_str());

			uint32_t messageId;
			try
			{
				messageId = std::stoi(value);
			}
			catch (std::exception)
			{
				skippedProperties.push_back(property);
				continue;
			}

			if (mMessageIds.count(messageId) == 0)
			{
				// Add the message id to the list
				mMessageIds.insert(std::pair<uint32_t, std::string>(messageId, property));
			}
			else
			{
				// Already have a key for that messageId, skip it
				skippedProperties.push_back(property);
				continue;
			}
		}
	}

	if (skippedProperties.size() > 0)
	{
		cConsoleLogger::LogMessage(eConsoleColor::YELLOW, L"The following %d properties could not be parsed:", skippedProperties.size());
		for (const std::string& property : skippedProperties)
		{
			cConsoleLogger::Log(L"-> %s\n", property.c_str());
		}
	}

	cConsoleLogger::LogMessage(eConsoleColor::WHITE, L"Read %d message ids from config file", mMessageIds.size());
	return true;
}
