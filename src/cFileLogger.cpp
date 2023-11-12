#include <cstdio>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "cFileLogger.h"

cFileLogger::cFileLogger(std::filesystem::path fsPath)
{
    fsPath.append(GenerateLogFilename());
    logStream = std::ofstream(fsPath, std::ios::out);
    if (logStream.is_open() == false)
	{
		throw std::runtime_error("Could not open file");
	}

	cLogger::RegisterLogger(this);

    // Print the time the log was generated at the top of the log file
    SYSTEMTIME st;
    GetSystemTime(&st);
    std::string logPreamble = std::string("SC4MessageViewer log file generated at " + std::to_string(st.wDay) + "/" + std::to_string(st.wMonth) + "/" + std::to_string(st.wYear) +
        " " + std::to_string(st.wHour) + ":" + std::to_string(st.wMinute)) + "\n\n";
    Log(&logPreamble, logPreamble.length());
}

cFileLogger::~cFileLogger()
{
}

void cFileLogger::Log(const std::string* message, DWORD length)
{
    if (logStream.is_open() == false)
	{
		return;
	}

    logStream << message->c_str();
}

std::wstring cFileLogger::GenerateLogFilename()
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    return std::wstring(L"SC4MessageViewer_" + std::to_wstring(st.wDay) + L"-" + std::to_wstring(st.wMonth) + L"-" + std::to_wstring(st.wYear) + 
        L"_" + std::to_wstring(st.wHour) + L"-" + std::to_wstring(st.wMinute) + L".log");
}
