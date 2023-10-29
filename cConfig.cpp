#include "cConfig.h"

#include <sstream>
#include <fstream>

bool cConfig::Load(std::string& sPath)
{
	std::ifstream fileStream(sPath.c_str());

	if (fileStream.fail())
	{
		return false;
	}

	std::string line;
	uint32_t skippedEntries;
	while (std::getline(fileStream, line))
	{
		// Ignore comment lines
		if (line.at(1) == ';')
		{
			continue;
		}

		std::size_t foundPosition = line.find('=');
		if (foundPosition != std::string::npos)
		{
			std::string key = line.substr(0, foundPosition - 1);
			std::string value = line.substr(foundPosition + 1, line.size());
			printf("%s:%s", key.c_str(), value.c_str());

			uint32_t messageId;
			try
			{
				messageId = std::stoi(value);
			}
			catch (std::invalid_argument)
			{

			}

			if (mMessageIds.count(messageId) != 0)
			{
				// Already have a key for that messageId, skip it
				skippedEntries++;
				continue;
			}

		}
	}

	return false;
}
