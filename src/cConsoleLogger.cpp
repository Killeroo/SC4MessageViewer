#include "cConsoleLogger.h"

#include <stdio.h>
#include <stdexcept>

#include "wil/result.h"

cConsoleLogger::cConsoleLogger() : hConsoleOutput(NULL), wCurrentConsoleColor(0)
{
    try
    {
        THROW_LAST_ERROR_IF(!AllocConsole());

        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        THROW_LAST_ERROR_IF(hConsoleOutput == INVALID_HANDLE_VALUE);

        if (!hConsoleOutput)
        {
            throw std::runtime_error("GetStdHandle returned a null handle.");
        }

        CONSOLE_SCREEN_BUFFER_INFO sbiConsoleInfo;
        // Get current console values if they haven't been set yet
        if (GetConsoleScreenBufferInfo(hConsoleOutput, &sbiConsoleInfo))
        {
            wCurrentConsoleColor = sbiConsoleInfo.wAttributes;
        }
        else
        {
            // Default colour if we couldn't get the current console buffer
            wCurrentConsoleColor = 8;
        }
    }
    catch (const wil::ResultException& e)
    {
        throw std::runtime_error(e.what());
    }
}

cConsoleLogger::~cConsoleLogger()
{
    // The OS will have closed the handle.
    hConsoleOutput = NULL;
}

void cConsoleLogger::LogMessage(eLogLevel level, const char* format, ...) const
{
    if (!hConsoleOutput)
    {
        return;
    }

    if (wCurrentConsoleColor != level)
    {
        wCurrentConsoleColor = level;
        SetConsoleTextAttribute(hConsoleOutput, wCurrentConsoleColor);
    }

    char formattedString[4096]{};

    // Fetch arguments and format them into output
    va_list args;
    va_start(args, format);
    int formattedStringCharCount = vsnprintf(formattedString, sizeof(formattedString), format, args);
    va_end(args);

    if (formattedStringCharCount > 0)
    {
        char outputString[6144]{};

        // Get current time
        SYSTEMTIME st;
        GetSystemTime(&st);

        // Print the time, log level and message.
        int outputStringCharCount = snprintf(outputString,
                                             sizeof(outputString),
                                             "[%hu:%hu:%hu.%hu] [%s] %s",
                                             st.wHour,
                                             st.wMinute,
                                             st.wSecond,
                                             st.wMilliseconds,
                                             mLogLevelStrings.at(level).c_str(), // Should check this level exists in the map but yeah...
                                             formattedString);
        if (outputStringCharCount > 0)
        {
            WriteFile(hConsoleOutput, outputString, static_cast<DWORD>(outputStringCharCount), NULL, NULL);
        }
    }

    // Don't better resetting, for a small performance gain maybe
    //SetConsoleTextAttribute(hConsoleOutputHandle, wCurrentConsoleColor);
}


void cConsoleLogger::Log(const char* format, ...) const
{
    if (!hConsoleOutput)
    {
        return;
    }

    char buffer[4096]{};

    va_list args;
    va_start(args, format);
    int charsWritten = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (charsWritten > 0)
    {
        WriteFile(hConsoleOutput, buffer, static_cast<DWORD>(charsWritten), NULL, NULL);
    }
}
