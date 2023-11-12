#pragma once

#include <Windows.h>
#include <vector>
#include <map>
#include <string>

#undef ERROR

enum eLogLevel : WORD
{
	EVENT = 15,
	INFO = 7,
	WARNING = 6,
	ERROR = 4,
	FATAL = 5,
};

const static std::map<WORD, std::string> mLogLevelStrings = {
	{eLogLevel::EVENT, std::string("EVENT")},
	{eLogLevel::INFO, std::string("INFO")},
	{eLogLevel::WARNING, std::string("WARNING")},
	{eLogLevel::ERROR, std::string("ERROR")},
	{eLogLevel::FATAL, std::string("FATAL")}
};

class iLogger
{
public:
	virtual void Log(const std::string* message, DWORD length) = 0;
};

class cLogger
{
public:
	static void RegisterLogger(iLogger* logger)
	{
		if (logger)
		{
			vLoggers.push_back(logger);
		}
	}

	static void DeregisterLogger(iLogger* logger)
	{
		// In this case we don't need to derigster the loggers as teardown of the loggers happens when the program ends.
		// But if you wanted to use this properly you would want to implement this
	}

	static void LogMessage(eLogLevel level, const char* format, ...)
	{
		char formattedString[4096]{};

		// Fetch arguments and format them into output
		va_list args;
		va_start(args, format);
		int formattedStringCharCount = vsnprintf(formattedString, sizeof(formattedString), format, args);
		va_end(args);

		if (formattedStringCharCount > 0)
		{
			char outputString[6144]{};

			// Get current time
			SYSTEMTIME st;
			GetSystemTime(&st);

			// Print the time, log level and message.
			int outputStringCharCount = snprintf(outputString,
				sizeof(outputString),
				"[%hu:%hu:%hu.%hu] [%s] %s",
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds,
				mLogLevelStrings.at(level).c_str(), // Should check this level exists in the map but yeah...
				formattedString);

			std::string outString = outputString;		
			for (iLogger* logger : vLoggers)
			{
				if (logger)
				{
					logger->Log(&outString, static_cast<DWORD>(outputStringCharCount));
				}
			}

		}
	}

	static void Log(const char* format, ...)
	{
		char buffer[4096]{};

		va_list args;
		va_start(args, format);
		int charsWritten = vsnprintf(buffer, sizeof(buffer), format, args);
		va_end(args);

		if (charsWritten > 0)
		{
			std::string outString = buffer;
			for (iLogger* logger : vLoggers)
			{
				if (logger)
				{
					logger->Log(&outString, static_cast<DWORD>(charsWritten));
				}
			}
		}
	}

private:
	inline static std::vector<iLogger*> vLoggers;

};