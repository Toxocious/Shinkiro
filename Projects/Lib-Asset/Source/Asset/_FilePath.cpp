#include <Asset/_Common.h>

#include <Asset/_FilePath.h>

namespace Shinkiro::Asset
{
    std::filesystem::path GetExecutablePath()
    {
        std::filesystem::path executablePath;

        wchar_t path[MAX_PATH] = { 0 };
        GetModuleFileNameW( NULL, path, MAX_PATH );
        executablePath = path;

        return executablePath;
    }

    std::filesystem::path GetExecutableDirectory()
    {
        return GetExecutablePath().parent_path();
    }
}
