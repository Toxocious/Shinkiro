#include <Asset/_FilePath.h>

fs::path GetExecutablePath()
{
    fs::path executablePath;

#if defined( _WIN32 ) || defined( _WIN64 )
    wchar_t path[MAX_PATH] = { 0 };
    GetModuleFileNameW( NULL, path, MAX_PATH );
    executablePath = path;

#elif defined( __APPLE__ )
    char     path[PATH_MAX];
    uint32_t size = PATH_MAX;
    if ( _NSGetExecutablePath( path, &size ) == 0 )
    {
        char realPath[PATH_MAX];
        if ( realpath( path, realPath ) )
        {
            executablePath = realPath;
        }
    }

#else
    char    path[PATH_MAX];
    ssize_t count = readlink( "/proc/self/exe", path, PATH_MAX );
    if ( count != -1 )
    {
        path[count]    = '\0';
        executablePath = path;
    }
#endif

    return executablePath;
}

fs::path GetExecutableDirectory()
{
    return GetExecutablePath().parent_path();
}
