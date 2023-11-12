#pragma once

#include <cstdio>
#include <iostream>
#include <fstream>

#include "cLogger.h"

class cFileLogger : public iLogger
{
public:
	cFileLogger(std::filesystem::path fsPath);
	~cFileLogger();

public:
    /** Begin iLogger interface */
    virtual void Log(const std::string* message, DWORD length) override;
    /** End iLogger interface */

private:
    std::wstring GenerateLogFilename();

private:
    std::ofstream logStream;
};