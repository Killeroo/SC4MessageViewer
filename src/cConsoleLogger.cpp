#include "cConsoleLogger.h"

#include <stdio.h>
#include <stdexcept>

#include "wil/result.h"

cConsoleLogger::cConsoleLogger() : hConsoleOutput(NULL), wCurrentConsoleColor(0)
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

    // Gray out close button on console window
    HWND consoleWindowHandle = GetConsoleWindow();
    HMENU consoleMenuHandle = GetSystemMenu(consoleWindowHandle, FALSE);
    EnableMenuItem(consoleMenuHandle, SC_CLOSE, MF_GRAYED);

    // Now that everything is created register with main logger class
    cLogger::RegisterLogger(this);
}

cConsoleLogger::~cConsoleLogger()
{
    // The OS will have closed the handle.
    hConsoleOutput = NULL;
}

void cConsoleLogger::Log(const std::string* message, DWORD length)
{
    if (!hConsoleOutput)
    {
        return;
    }

    WriteFile(hConsoleOutput, message->c_str(), length, NULL, NULL);
}