#include "cConsoleLogger.h"
#include <cstdio>

HANDLE cConsoleLogger::hConsoleOutputHandle = NULL;
CONSOLE_SCREEN_BUFFER_INFO cConsoleLogger::sbiConsoleInfo;
WORD cConsoleLogger::wCurrentConsoleColor = 0;

void cConsoleLogger::Init()
{
    if (AllocConsole())
    {
        // Redirect stdout to the newly created console.
        freopen("CONOUT$", "w", stdout); // Use hConsoleOutputHandle instead

        printf("-> Console created\n");


        // Get current console values if they haven't been set yet
        hConsoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE); // TODO: Move to init
        if (GetConsoleScreenBufferInfo(hConsoleOutputHandle, &sbiConsoleInfo))
        {
            wCurrentConsoleColor = sbiConsoleInfo.wAttributes;
        }
        else
        {
            // Default colour if we couldn't get the current console buffer
            wCurrentConsoleColor = 8;
        }
    }
}

void cConsoleLogger::Teardown()
{

    CloseHandle(hConsoleOutputHandle);

    if (FreeConsole())
    {
    }

}

void cConsoleLogger::LogMessage(eConsoleColor color, const wchar_t* format, ...)
{
    // set our colour
    SetConsoleTextAttribute(hConsoleOutputHandle, color);

    // Get current time
    SYSTEMTIME st;
    GetSystemTime(&st);

    // Print time
    printf("[%hu:%hu:%hu.%hu] \n",
        st.wHour,
        st.wMinute,
        st.wSecond,
        st.wMilliseconds);

    // Fetch arguments and format them into output
    va_list args;
    va_start(args, format);
    vfwprintf(stdout, format, args);
    va_end(args);

    // Cleanup the colour after
    SetConsoleTextAttribute(hConsoleOutputHandle, wCurrentConsoleColor);
}
