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

  #include <GL/gl.h>
#endif

#ifndef SHINKIRO_LOGGER_HEADERS
#    define SHINKIRO_LOGGER_HEADERS
#    include <Core/Util/Log.h>

#    include <Core/Util/Assert.h>
#    include <Core/Util/Macro.h>
#endif
