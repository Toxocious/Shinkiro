#include <Core/_Common.h>

#include <Core/Util/FileSystem.h>

namespace Shinkiro::Core
{
    std::filesystem::path FileSystem::GetExecutablePath()
    {
        std::filesystem::path executablePath;

        wchar_t path[MAX_PATH] = { 0 };
        GetModuleFileNameW( NULL, path, MAX_PATH );
        executablePath = path;

        return executablePath;
    }

    std::filesystem::path FileSystem::GetExecutableDirectory()
    {
        return GetExecutablePath().parent_path();
    }

    std::filesystem::path FileSystem::GetCoreAssetPath()
    {
        char exePath[MAX_PATH];
        GetModuleFileNameA( NULL, exePath, MAX_PATH );
        std::filesystem::path executablePath = std::filesystem::path( exePath ).parent_path();

        std::filesystem::path assetPath = executablePath / "Assets";
        if ( std::filesystem::exists( assetPath ) )
        {
            return assetPath;
        }

        SHNK_CORE_ERROR( "Could not find assets directory!" );
        return executablePath;
    }

    std::string FileSystem::GetFileExtension( const char * file, bool with_dot )
    {
        std::string f( file );
        std::string ext;

        for ( auto i = f.rbegin(); i != f.rend(); ++i )
        {
            if ( *i != '.' )
            {
                ext.push_back( std::tolower( *i ) );
            }
            else
            {
                if ( with_dot )
                {
                    ext.push_back( std::tolower( *i ) );
                }
                break;
            }
        }

        std::reverse( ext.begin(), ext.end() );

        return ext;
    }

    uint64_t FileSystem::LastTimeWrite( const char * path )
    {
        struct stat time;

        if ( stat( path, &time ) == 0 )
        {
            return time.st_mtime;
        }

        return 0ULL;
    }
}
