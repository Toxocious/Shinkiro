#pragma once

#ifndef SHINKIRO_ASSET_FILEPATH_H
#    define SHINKIRO_ASSET_FILEPATH_H

#    include <Asset/_Defs.h>

#    include <filesystem>

namespace Shinkiro::Asset
{
    /**
     * @brief Gets the full path to the executable
     * @return The file path to the executable
     */
    ASSET_API std::filesystem::path GetExecutablePath();

    /**
     * @brief Gets the directory the executable is located in
     * @return The file path to the directory
     */
    ASSET_API std::filesystem::path GetExecutableDirectory();
}

#endif
