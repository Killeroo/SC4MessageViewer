#include "cFileHelper.h"

#include <Windows.h>
#include "wil/resource.h"
#include "wil/filesystem.h"

std::filesystem::path cFileHelper::GetCurrentModuleDirectory()
{
    wil::unique_cotaskmem_string modulePath = wil::GetModuleFileNameW(wil::GetModuleInstanceHandle());

    std::filesystem::path temp(modulePath.get());

    return temp.parent_path();
}

bool cFileHelper::DoesFileExist(const std::filesystem::path& path)
{
    const DWORD fileType = GetFileAttributes(path.c_str());

    // Pretty broad check, but should work for what we need to do
    return fileType != INVALID_FILE_ATTRIBUTES;
}
