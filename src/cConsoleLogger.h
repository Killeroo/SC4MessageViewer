#pragma once

#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <type_traits>
#include <map>

#include "cLogger.h"

class cConsoleLogger : public iLogger
{
public:
    cConsoleLogger();
    ~cConsoleLogger();

public: 
    /** Begin iLogger interface */
    virtual void Log(const std::string* message, DWORD length) override;
    /** End iLogger interface */

private:
    HANDLE hConsoleOutput;
    mutable WORD wCurrentConsoleColor;
};
