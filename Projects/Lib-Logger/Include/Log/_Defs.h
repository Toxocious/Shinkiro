#pragma once

#ifdef PLATFORM_WINDOWS
#    ifdef SHINKIRO_LOGGER_EXPORTS
#        define LOG_API __declspec( dllexport )
#    else
#        define LOG_API __declspec( dllimport )
#    endif
#else
#    define LOG_API
#endif
