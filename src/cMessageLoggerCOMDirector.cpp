#include "cMessageLoggerCOMDirector.h"

#include "include/cIGZFrameWork.h"
#include "include/cIGZMessage2Standard.h"
#include "include/cIGZMessageServer2.h"
#include "include/GZServPtrs.h"

#include "cConsoleLogger.h"
#include "cFileHelper.h"
#include "cConfig.h"
#include "Constants.h"

#include <Windows.h>

cMessageLoggerCOMDirector::cMessageLoggerCOMDirector() : config(), consoleLogger()
{
	//MessageBoxA(NULL, "Attach Debugger", "Sc4MessageViewer", MB_OK | MB_ICONERROR);

	// Try and read from the config file
	const std::filesystem::path configFilePath = cFileHelper::GetCurrentModuleDirectory().append(CONFIG_FILE_NAME);
	if (cFileHelper::DoesFileExist(configFilePath) == false)
	{
		if (config.CreateDefault(configFilePath) == false)
		{
			MessageBoxA(NULL, "Could not create default config file", "Sc4MessageViewer", MB_OK | MB_ICONERROR);
		}
	}

	// Load our config
	if (config.Load(configFilePath) == false)
	{
		MessageBoxA(NULL, "Could not read config file", "Sc4MessageViewer", MB_OK | MB_ICONERROR);
		return;
	}

	// Setup loggers
	if (config.LogToConsole())
	{
		try
		{
			consoleLogger = std::make_unique<cConsoleLogger>();
		}
		catch (const std::exception& e)
		{
			MessageBoxA(NULL, e.what(), "Sc4MessageViewer - Could not create console", MB_OK | MB_ICONERROR);
		}
	}
	if (config.LogToFile())
	{
		try
		{
			fileLogger = std::make_unique<cFileLogger>(cFileHelper::GetCurrentModuleDirectory());
		}
		catch (const std::exception& e)
		{
			MessageBoxA(NULL, e.what(), "Sc4MessageViewer - Could not create log file", MB_OK | MB_ICONERROR);
		}
	}

	// Print out some log preamble about what we read in the config file
	cLogger::Log(PROGRAM_NAME);
	const std::vector<std::string>& skippedMessages = config.GetSkippedMessages();
	if (skippedMessages.size() > 0)
	{
		cLogger::LogMessage(eLogLevel::WARNING, "%d message ids could not be parsed:\n", skippedMessages.size());
		for (const std::string& property : skippedMessages)
		{
			cLogger::Log("-> %s\n", property.c_str());
		}
	}
	cLogger::LogMessage(eLogLevel::INFO, "Read %d message ids from config file\n", config.GetMessageIds().size());
}

cMessageLoggerCOMDirector::~cMessageLoggerCOMDirector()
{
}

bool cMessageLoggerCOMDirector::DoMessage(cIGZMessage2* pMessage)
{
	cIGZMessage2Standard* pStandardMessage = static_cast<cIGZMessage2Standard*>(pMessage);
	uint32_t dwType = pMessage->GetType();

	if (config.GetMessageIds().count(dwType) != 0)
	{
		cLogger::LogMessage(eLogLevel::EVENT, "%s\n", config.GetMessageIds().at(dwType).c_str());
	}

	return true;
}

bool cMessageLoggerCOMDirector::OnStart(cIGZCOM* pCOM)
{
	RZGetFrameWork()->AddHook(static_cast<cIGZFrameWorkHooks*>(this));
	return true;
}

bool cMessageLoggerCOMDirector::PostAppInit(void)
{
	cIGZMessageServer2Ptr pMessageServer;
	if (!pMessageServer)
	{
		cLogger::LogMessage(eLogLevel::ERROR, "Could not fetch message server.\n");
		return true;
	}
	if (config.GetMessageIds().size() == 0)
	{
		cLogger::LogMessage(eLogLevel::ERROR, "No valid message ids have been parsed. Nothing to listen too.\n");
		return true;
	}

	// Loop through parsed message ids and try and add notifications for them
	uint32_t registeredMessages = 0;
	for (const std::pair<uint32_t, std::string>& entry : config.GetMessageIds())
	{
		if (pMessageServer->AddNotification(this, entry.first))
		{
			registeredMessages++;
		}
		else
		{
			cLogger::LogMessage(eLogLevel::WARNING, "Failed to register message: %s (%d)\n", entry.second.c_str(), entry.first);
		}
	}
	cLogger::LogMessage(eLogLevel::INFO, "Successfully registered %d/%d messages\n", registeredMessages, config.GetMessageIds().size());

	return true;
}

