#pragma once

#ifndef SHINKIRO_ASSET_FILEPATH_H
#    define SHINKIRO_ASSET_FILEPATH_H

#    include <Asset/_Defs.h>

#    include <filesystem>

namespace Shinkiro::Asset
{
    ASSET_API std::filesystem::path GetExecutablePath();
    ASSET_API std::filesystem::path GetExecutableDirectory();
}

#endif
