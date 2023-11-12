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

bool cConfig::Load(const std::wstring& sPath)
{
	std::ifstream fileStream(sPath.c_str());
	if (fileStream.fail())
	{
		cLogger::LogMessage(eLogLevel::ERROR, "Failed to read config file\n");
		return false;
	}

	// Got through the config file line by line
	std::string currentSection;
	std::string line;
	while (std::getline(fileStream, line))
	{
		// Ignore comment of blank lines
		if (line.size() == 0 || line.at(0) == ';')
		{
			continue;
		}

		// Try and find currentSection label
		const std::size_t sectionHeaderPos = line.find('[');
		if (sectionHeaderPos != std::string::npos)
		{
			const std::size_t startPos = line.find_first_of('[');
			const std::size_t endPos = line.find_first_of(']');
			if (endPos < startPos)
			{
				// Ignore a malformed currentSection
				continue;
			}

			std::string section = line.substr(startPos + 1, endPos - 1);
			if (section.empty() == false)
			{
				StringToLower(&section);

				// Save current section
				currentSection = section;
			}

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
			std::string key = untrimmedKey.substr(startPos, endPos);

			// Extract value (excluding any midline comment)
			const std::size_t commentPos = line.find(';');
			const std::size_t valueLen = (commentPos == std::string::npos ? line.size() : commentPos + 1) - separatorPos;
			std::string value = line.substr(separatorPos + 1, valueLen);

			// Treat the property as a message by default
			if (currentSection.empty() || currentSection == "messages")
			{
				// Try convert value
				uint32_t messageId;
				try
				{
					messageId = std::stoul(value, nullptr, 16);

					if (messageId == 0)
					{
						vSkippedMessages.push_back(key);
						continue;
					}
				}
				catch (std::exception)
				{
					vSkippedMessages.push_back(key);
					continue;
				}

				// Try add message id to the map
				if (mMessageIds.count(messageId) == 0)
				{
					mMessageIds.insert(std::pair<uint32_t, std::string>(messageId, key));
				}
				else
				{
					vSkippedMessages.push_back(key);
					continue;
				}
			}
			else if (currentSection == "setup")
			{
				// Convert key and string to lowercase
				StringToLower(&key);
				StringToLower(&value);

				// We need to trim the value as well in this case
				const std::size_t startPos = (std::max)(value.find_first_not_of('\t'), value.find_first_not_of(' '));
				const std::size_t endPos = (std::min)(value.find_last_not_of('\t'), value.find_last_not_of(' ')) + 1;
				value = value.substr(startPos, endPos);

				// Save some specific setup properties (ugly I know..)
				if (key == "consolelog")
				{
					if (value == "true")
					{
						bLogToConsole = true;
					}
					else if (value == "false")
					{
						bLogToConsole = false;
					}
				}
				else if (key == "filelog")
				{
					std::string trueString = std::string("true");
					if (value == trueString)
					{
						bLogToFile = true;
					}
					else if (value == std::string("false"))
					{
						bLogToFile = false;
					}
				}
			}
		}
	}

	return true;
}

void cConfig::StringToLower(std::string* string)
{
	// Convert to lower (https://stackoverflow.com/a/313990)
	std::transform(string->begin(), string->end(), string->begin(),
		[](unsigned char c) { return std::tolower(c); });
}

