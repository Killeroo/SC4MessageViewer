#pragma once

#include <Windows.h>

enum eConsoleColor : WORD
{
    GREEN = 2,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7
};

class cConsoleLogger
{
public:
	static bool Init();
	static void Teardown();
    static void LogMessage(eConsoleColor color, const wchar_t* format, ...);
    static void Log(const wchar_t* format, ...);

private:
    static HANDLE hConsoleOutputHandle;
    static CONSOLE_SCREEN_BUFFER_INFO sbiConsoleInfo;
    static WORD wCurrentConsoleColor;
};