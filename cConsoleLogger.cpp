#include "cConsoleLogger.h"

#include <cstdio>
#include <iostream>
#include <string>

HANDLE cConsoleLogger::hConsoleOutput = NULL;
CONSOLE_SCREEN_BUFFER_INFO cConsoleLogger::sbiConsoleInfo;
WORD cConsoleLogger::wCurrentConsoleColor = 0;

bool cConsoleLogger::Init()
{
    if (AllocConsole())
    {
        // Redirect stdout to the newly created console.
        freopen("CONOUT$", "w", stdout); // Use hConsoleOutput instead

        // Get current console values if they haven't been set yet
        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE); // TODO: Move to init
        if (GetConsoleScreenBufferInfo(hConsoleOutput, &sbiConsoleInfo))
        {
            wCurrentConsoleColor = sbiConsoleInfo.wAttributes;
        }
        else
        {
            // Default colour if we couldn't get the current console buffer
            wCurrentConsoleColor = 8;
        }

        return true;
    }

    return false;
}

void cConsoleLogger::Teardown()
{
    CloseHandle(hConsoleOutput);
    FreeConsole();
}

void cConsoleLogger::LogMessage(eLogLevel level, const char* format, ...)
{
    if (wCurrentConsoleColor != level)
    {
        wCurrentConsoleColor = level;
        SetConsoleTextAttribute(hConsoleOutput, wCurrentConsoleColor);
    }

    // Get current time
    SYSTEMTIME st;
    GetSystemTime(&st);

    // Print time
    printf("[%hu:%hu:%hu.%hu] [%s] ",
        st.wHour,
        st.wMinute,
        st.wSecond,
        st.wMilliseconds,
        mLogLevelStrings.at(level).c_str()); // Should check this level exists in the map but yeah...

    // Fetch arguments and format them into output
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);

    // Don't better resetting, for a small performance gain maybe
    //SetConsoleTextAttribute(hConsoleOutputHandle, wCurrentConsoleColor);
}

void cConsoleLogger::Log(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}