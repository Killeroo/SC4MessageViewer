#include "cMessageLoggerCOMDirector.h"

#include "include/cIGZFrameWork.h"
#include "include/cIGZMessage2Standard.h"

#include "cConsoleLogger.h"
#include "cFileHelper.h"
#include "cConfig.h"

#include <Windows.h>

cMessageLoggerCOMDirector::cMessageLoggerCOMDirector()
{
	// Setup the console for logging
	if (cConsoleLogger::Init())
	{
		cConsoleLogger::LogMessage(eConsoleColor::YELLOW, L"Sc4MessageViewer v1.0");
	}
	else
	{
		MessageBoxA(NULL, "Could not create console..", "Sc4MessageViewer", MB_OK);
	}

	// Try and read from the config file
	const std::wstring configFilePath = cFileHelper::GetCurrentPath() + L"\\" + L"config.ini";
	cConsoleLogger::LogMessage(WHITE, L"Loading %s", configFilePath.c_str());

	cConfig config;
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

	//Log(APP, "Recieved message type=" + dwType);

	return true;
}

bool cMessageLoggerCOMDirector::OnStart(cIGZCOM* pCOM)
{
	RZGetFrameWork()->AddHook(static_cast<cIGZFrameWorkHooks*>(this));
	return true;
}

bool cMessageLoggerCOMDirector::PostAppInit(void)
{
	printf("textextext");
	cConsoleLogger::LogMessage(eConsoleColor::MAGENTA, L"This is a test");
	return true;
}

