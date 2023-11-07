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

cMessageLoggerCOMDirector::cMessageLoggerCOMDirector() : config(), logger()
{
	// Setup the console for logging
	try
	{
		logger = std::make_unique<cConsoleLogger>();
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Sc4MessageViewer - Could not create console", MB_OK | MB_ICONERROR);
	}

	if (logger)
	{
		logger->Log(PROGRAM_NAME);
	}

	// Try and read from the config file
	const std::filesystem::path configFilePath = cFileHelper::GetCurrentModuleDirectory().append(CONFIG_FILE_NAME);
	if (cFileHelper::DoesFileExist(configFilePath) == false)
	{
		if (config.CreateDefault(configFilePath) == false)
		{
			if (logger)
			{
				logger->LogMessage(eLogLevel::ERROR, "Could not create default config file\n");
			}
		}
	}

	// Load our config
	config.Load(configFilePath, logger.get());
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
		if (logger)
		{
			logger->LogMessage(eLogLevel::EVENT, "%s\n", config.GetMessageIds().at(dwType).c_str());
		}
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
		if (logger)
		{
			logger->LogMessage(eLogLevel::ERROR, "Could not fetch message server.\n");
		}
		return true;
	}
	if (config.GetMessageIds().size() == 0)
	{
		if (logger)
		{
			logger->LogMessage(eLogLevel::ERROR, "No valid message ids have been parsed. Nothing to listen too.\n");
		}
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
			if (logger)
			{
				logger->LogMessage(eLogLevel::WARNING, "Failed to register message: %s (%d)\n", entry.second.c_str(), entry.first);
			}
		}
	}
	if (logger)
	{
		logger->LogMessage(eLogLevel::INFO, "Successfully registered %d/%d messages\n", registeredMessages, config.GetMessageIds().size());
	}

	return true;
}

