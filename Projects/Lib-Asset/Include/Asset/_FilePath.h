#pragma once

#ifndef SHINKIRO_ASSET_FILEPATH_H
#    define SHINKIRO_ASSET_FILEPATH_H

#    include <Asset/_Defs.h>

#    if defined( _WIN32 ) || defined( _WIN64 )
#        include <windows.h>
#    elif defined( __APPLE__ )
#        include <limits.h>
#        include <mach-o/dyld.h>
#    else
#        include <limits.h>
#        include <unistd.h>
#    endif

#    include <filesystem>
#    include <string>

namespace Shinkiro::Asset
{
    ASSET_API std::filesystem::path GetExecutablePath();
    ASSET_API std::filesystem::path GetExecutableDirectory();
}

#endif
