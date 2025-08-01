#pragma once

#ifndef SHINKIRO_CORE_FILE_SYSTEM_H
#    define SHINKIRO_CORE_FILE_SYSTEM_H

#    include <Core/_Defs.h>

#    include <Core/_Common.h>

namespace Shinkiro::Core
{
    class FileSystem
    {
    public:
        static std::string OpenTextFile( const char * path );

        static bool Exists( const char * path );

        static uint64_t LastTimeWrite( const char * path );

        static std::string GetFileExtension( const char * file, bool with_dot = false );
        static std::string GetFullPath( const char * path );

        static std::filesystem::path GetCoreAssetPath();

        static std::string NormalizePath( const std::string & path );
    };
}

#endif
