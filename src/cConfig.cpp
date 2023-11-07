#include "cConfig.h"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>

#include "cConsoleLogger.h"
#include "Constants.h"

bool cConfig::CreateDefault(const std::wstring& sPath)
{
	std::ofstream fileStream(sPath, std::ios::out);
	if (fileStream.is_open() == false)
	{
		return false;
	}

	fileStream << DEFAULT_CONFIG_FILE_CONTENTS;
	return true;
}

bool cConfig::Load(const std::wstring& sPath, const cConsoleLogger* logger)
{
	std::ifstream fileStream(sPath.c_str());
	if (fileStream.fail())
	{
		if (logger)
		{
			logger->LogMessage(eLogLevel::ERROR, "Failed to read config file\n");
		}
		return false;
	}

	// Got through the config file line by line
	std::string line;
	std::vector<std::string> skippedProperties;
	while (std::getline(fileStream, line))
	{
		// Ignore comment of blank lines
		if (line.size() == 0 || line.at(0) == ';')
		{
			continue;
		}

		// Try and seperate each line into the key and it's index
		const std::size_t separatorPos = line.find('=');
		if (separatorPos != std::string::npos)
		{
			// Extract key (trim any leading or trailing blank spaces or tabs)
			const std::string untrimmedKey = line.substr(0, separatorPos);
			const std::size_t startPos = (std::max)(untrimmedKey.find_first_not_of('\t'), untrimmedKey.find_first_not_of(' '));
			const std::size_t endPos = (std::min)(untrimmedKey.find_last_not_of('\t'), untrimmedKey.find_last_not_of(' ')) + 1;
			const std::string key = untrimmedKey.substr(startPos, endPos);

			// Extract value (excluding any midline comment)
			const std::size_t commentPos = line.find(';');
			const std::size_t valueLen = (commentPos == std::string::npos ? line.size() : commentPos + 1) - separatorPos;
			const std::string value = line.substr(separatorPos + 1, valueLen);

			// Try convert value
			uint32_t messageId;
			try
			{
				messageId = std::stoul(value, nullptr, 16);

				if (messageId == 0)
				{
					skippedProperties.push_back(key);
					continue;
				}
			}
			catch (std::exception)
			{
				skippedProperties.push_back(key);
				continue;
			}

			// Try add message id to the map
			if (mMessageIds.count(messageId) == 0)
			{
				mMessageIds.insert(std::pair<uint32_t, std::string>(messageId, key));
			}
			else
			{
				skippedProperties.push_back(key);
				continue;
			}
		}
	}

	if (logger)
	{
		if (skippedProperties.size() > 0)
		{
			logger->LogMessage(eLogLevel::WARNING, "%d message ids could not be parsed:\n", skippedProperties.size());
			for (const std::string& property : skippedProperties)
			{
				logger->Log("-> %s\n", property.c_str());
			}
		}

		logger->LogMessage(eLogLevel::INFO, "Read %d message ids from config file\n", mMessageIds.size());
	}

	return true;
}

