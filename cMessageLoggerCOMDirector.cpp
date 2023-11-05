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

cMessageLoggerCOMDirector::cMessageLoggerCOMDirector()
{
	// Setup the console for logging
	if (cConsoleLogger::Init())
	{
		cConsoleLogger::LogMessage(eLogLevel::INFO, PROGRAM_NAME);
	}
	else
	{
		MessageBoxA(NULL, "Could not create console..", "Sc4MessageViewer", MB_OK);
	}

	// Try and read from the config file
	// (When we use GetCurrentPath, we are retrieving the location of "SimCity 4.exe" so need to do some fiddling to get the plugin path)
	const std::wstring configFilePath = cFileHelper::GetParentDirectory(cFileHelper::GetCurrentPath()) + L"\\" + L"Plugins" + L"\\" + CONFIG_FILE_NAME;
	if (cFileHelper::DoesFileExist(configFilePath) == false)
	{
		if (config.CreateDefault(configFilePath) == false)
		{
			cConsoleLogger::LogMessage(eLogLevel::ERROR, "Could not create default config file\n");
		}
	}

	// Load our config
	config.Load(configFilePath);
}

cMessageLoggerCOMDirector::~cMessageLoggerCOMDirector()
{
	cConsoleLogger::Teardown();
}

bool cMessageLoggerCOMDirector::DoMessage(cIGZMessage2* pMessage)
{
	cIGZMessage2Standard* pStandardMessage = static_cast<cIGZMessage2Standard*>(pMessage);
	uint32_t dwType = pMessage->GetType();

	if (config.GetMessageIds().count(dwType) != 0)
	{
		cConsoleLogger::LogMessage(eLogLevel::EVENT, "%s\n", config.GetMessageIds().at(dwType).c_str());
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
		cConsoleLogger::LogMessage(eLogLevel::ERROR, "Could not fetch message server.\n");
		return true;
	}
	if (config.GetMessageIds().size() == 0)
	{
		cConsoleLogger::LogMessage(eLogLevel::ERROR, "No valid message ids have been parsed. Nothing to listen too.\n");
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
			cConsoleLogger::LogMessage(eLogLevel::WARNING, "Failed to register message: %s (%d)\n", entry.second.c_str(), entry.first);
		}
	}
	cConsoleLogger::LogMessage(eLogLevel::INFO, "Successfully registered %d/%d messages\n", registeredMessages, config.GetMessageIds().size());

	return true;
}

