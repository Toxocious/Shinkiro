#pragma once

#ifdef PLATFORM_WINDOWS
#    ifdef SHINKIRO_AUDIO_EXPORTS
#        define AUDIO_API __declspec( dllexport )
#    else
#        define AUDIO_API __declspec( dllimport )
#    endif
#else
#    define AUDIO_API
#endif
