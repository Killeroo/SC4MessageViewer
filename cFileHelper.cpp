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

std::wstring cFileHelper::GetParentDirectory(std::wstring path)
{
    // Warning: We are going to make a lot of assumptions about the format of the path
    // this works for us but will not be robust enough to be used in many other applications

    // Strip any leading directory seperator 
    if (path.back() == '\\')
    {
        path.erase(path.size() - 1, 1);
    }

    return path.substr(0, path.find_last_of('\\'));
}

bool cFileHelper::DoesFileExist(const std::wstring& path)
{
    const DWORD fileType = GetFileAttributes(path.c_str());

    // Pretty broad check, but should work for what we need to do
    return fileType != INVALID_FILE_ATTRIBUTES;
}
