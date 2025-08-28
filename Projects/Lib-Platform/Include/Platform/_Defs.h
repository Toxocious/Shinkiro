#pragma once

#ifdef PLATFORM_WINDOWS
#    ifdef SHINKIRO_PLATFORM_EXPORTS
#        define PLATFORM_API __declspec( dllexport )
#    else
#        define PLATFORM_API __declspec( dllimport )
#    endif
#else
#    define PLATFORM_API
#endif

#ifndef SHINKIRO_IMPL_GLAD
#    define SHINKIRO_IMPL_GLAD
#    include <glad/glad.h>
#endif

#ifndef SHINKIRO_IMPL_GLFW
#    define SHINKIRO_IMPL_GLFW
#    include <glfw/glfw3.h>

#    include <gl/GL.h>
#endif

#ifndef SHINKIRO_LOGGER_HEADERS
#    define SHINKIRO_LOGGER_HEADERS
#    include <Log/Log.h>
#    include <Log/Util/Assert.h>
#    include <Log/Util/Macro.h>
#endif

namespace Shinkiro::Platform
{
    enum class PLATFORM_API UpdateStatus
    {
        UPDATE_CONTINUE,
        UPDATE_STOP,
        UPDATE_ERROR
    };
}
