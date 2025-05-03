#ifdef PLATFORM_WINDOWS
#    ifdef CORE_ASSET_EXPORTS
#        define ASSET_API __declspec( dllexport )
#    else
#        define ASSET_API __declspec( dllimport )
#    endif
#else
#    define ASSET_API
#endif

#pragma once

#if defined( _WIN32 ) || defined( _WIN64 )
#    include <windows.h>
#elif defined( __APPLE__ )
#    include <limits.h>
#    include <mach-o/dyld.h>
#else // Linux and other Unix-like
#    include <limits.h>
#    include <unistd.h>
#endif

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

ASSET_API fs::path GetExecutablePath();
ASSET_API fs::path GetExecutableDirectory();
