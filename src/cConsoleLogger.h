#pragma once

#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <type_traits>
#include <map>

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

class cConsoleLogger
{
public:
	static bool Init();
	static void Teardown();
    static void Log(const char* format, ...);
    static void LogMessage(eLogLevel level, const char* format, ...);

private:
    static HANDLE hConsoleOutput;
    static CONSOLE_SCREEN_BUFFER_INFO sbiConsoleInfo;
    static WORD wCurrentConsoleColor;
};
