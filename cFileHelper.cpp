#include "cFileHelper.h"

#include <Windows.h>

// Based off: https://stackoverflow.com/a/875264
std::wstring cFileHelper::GetCurrentPath()
{
    WCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    const std::wstring fullPath = std::wstring(buffer);

    const std::wstring::size_type lastDirSeparatorPos = fullPath.find_last_of(L'\\/');
    if (lastDirSeparatorPos != std::wstring::npos)
    {
        return fullPath.substr(0, lastDirSeparatorPos);
    }
    else
    {
        return fullPath;
    }
}
