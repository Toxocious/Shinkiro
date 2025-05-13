#pragma once

#ifdef PLATFORM_WINDOWS
#    ifdef SHINKIRO_RENDERER_EXPORTS
#        define RENDER_API __declspec( dllexport )
#    else
#        define RENDER_API __declspec( dllimport )
#    endif
#else
#    define RENDER_API
#endif
