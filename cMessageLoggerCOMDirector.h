#pragma once

#include <stdio.h>
#include <iostream>
#include <thread>

#include "include/cRZCOMDllDirector.h"
#include "include/cRZMessage2COMDirector.h"
#include "include/cIGZMessage2.h"
#include "include/cIGZFrameWork.h"

#include "cConfig.h"

static const uint32_t kMessageLoggerDirectorId = 0xD81A8F98;

class cMessageLoggerCOMDirector : public cRZMessage2COMDirector
{
public:
	cMessageLoggerCOMDirector();
	virtual ~cMessageLoggerCOMDirector();

public:
	uint32_t GetDirectorID() const { return kMessageLoggerDirectorId; }

	/** Callback for SC4 messages */
	bool DoMessage(cIGZMessage2* pMessage);

	// Director Events
	bool OnStart(cIGZCOM* pCOM);
	bool PostAppInit();

private:
	cConfig config;
	bool bSetup = false;
};

// DLL entry point
cRZCOMDllDirector* RZGetCOMDllDirector()
{
	static cMessageLoggerCOMDirector sDirector;
	return &sDirector;
}