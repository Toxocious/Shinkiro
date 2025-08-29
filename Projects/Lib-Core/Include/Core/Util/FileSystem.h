#pragma once

#ifndef SHINKIRO_CORE_FILE_SYSTEM_H
#    define SHINKIRO_CORE_FILE_SYSTEM_H

#    include <Core/_Defs.h>

#    include <filesystem>
#    include <string>

namespace Shinkiro::Core
{
    class CORE_API FileSystem
    {
    public:
        /**
         * @brief Gets the fs::path to the current executable.
         * @return The executable path.
         */
        static std::filesystem::path GetExecutablePath();

        /**
         * @brief Gets the fs::path to the directory containing the executable.
         * @return The directory path.
         */
        static std::filesystem::path GetExecutableDirectory();

        /**
         * @brief Gets a fs::path to the /Assets folder relative to the executable.
         * @return The asset path.
         */
        static std::filesystem::path GetCoreAssetPath();

        /**
         * @brief Gets the last time a file was writen at.
         * @param path The file path.
         */
        static uint64_t LastTimeWrite( const char * path );

        /**
         * @brief Normalizes a given file path.
         * @param path The file path.
         */
        static std::string GetFileExtension( const char * file, bool with_dot = false );
    };
}

#endif
