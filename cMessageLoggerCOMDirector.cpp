#include "cMessageLoggerCOMDirector.h"

#include "include/cIGZFrameWork.h"
#include "include/cIGZMessage2Standard.h"

#include "cConsoleLogger.h"

#include <Windows.h>

cMessageLoggerCOMDirector::cMessageLoggerCOMDirector()
{
	if (AllocConsole())
	{
		// Redirect stdout to the newly created console.
		freopen("CONOUT$", "w", stdout);

		printf("-> Console created\n");

		// TODO: Free console on destructor
	}
}

cMessageLoggerCOMDirector::~cMessageLoggerCOMDirector()
{
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

