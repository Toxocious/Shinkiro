#include <Core/Util/FileSystem.h>

#include <filesystem>
#include <fstream>
#include <stack>

namespace Shinkiro::Core
{
    namespace fs = std::filesystem;

    /**
     * Returns a fs::path to the /Assets folder relative to the executable.
     */
    std::filesystem::path FileSystem::GetCoreAssetPath()
    {
        namespace fs = std::filesystem;

        char exePath[MAX_PATH];
        GetModuleFileNameA( NULL, exePath, MAX_PATH );
        fs::path executablePath = fs::path( exePath ).parent_path();

        fs::path assetPath = executablePath / "Assets";
        if ( fs::exists( assetPath ) )
        {
            return assetPath;
        }

        SHNK_CORE_ERROR( "Could not find assets directory!" );
        return executablePath;
    }

    /**
     * Returns a string containing the full OS path.
     */
    std::string FileSystem::GetFullPath( const char * path )
    {
        return fs::absolute( path ).string();
    }

    /**
     * Opens a text file and returns its contents.
     */
    std::string FileSystem::OpenTextFile( const char * path )
    {
        try
        {
            std::ifstream in( path, std::ios::in | std::ios::binary );
            if ( in )
            {
                std::string contents;

                in.seekg( 0, std::ios::end );
                contents.resize( in.tellg() );
                in.seekg( 0, std::ios::beg );
                in.read( &contents[0], contents.size() );
                in.close();

                return contents;
            }
            throw path;
        }
        catch ( ... )
        {
            SHNK_CORE_WARN( "Cannot open file with path '{0}'", path );
            throw path;
        }
    }

    /**
     * Returns a string containing the type of extension of a file.
     */
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

    /**
     * Normalizes a given file path.
     */
    std::string FileSystem::NormalizePath( const std::string & path )
    {
        std::string ret;

        auto i = path.begin();
        while ( i != path.end() )
        {
            if ( *i == '\\' )
            {
                ret.push_back( '/' );
            }
            else
            {
                ret.push_back( *i );
            }

            ++i;
        }

        return ret;
    }

    /**
     * Checks to see if a given file path exists.
     */
    bool FileSystem::Exists( const char * path )
    {
        try
        {
            return fs::exists( path );
        }
        catch ( const std::filesystem::filesystem_error & e )
        {
            SHNK_CORE_WARN( "Failed to check if the path exists due to '{0}'", e.what() );
        }

        return false;
    }

    /**
     * Returns the last time at which a file was written to.
     */
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
