#pragma once

#include <filesystem>
#include <iostream>
#include <memory>

#if defined _WIN32
#    include <Windows.h>

#    if !defined( assert )
#        include <stdio.h>
#        define assert( x )                                      \
            do                                                   \
            {                                                    \
                if ( !( x ) )                                    \
                {                                                \
                    printf( "Error: assert(%s) failed!\n", #x ); \
                }                                                \
            } while ( 0 )
#    endif
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

#ifndef SHINKIRO_IMPL_SPDLOG
#    define SHINKIRO_IMPL_SPDLOG
#    pragma warning( push, 0 )
#    ifdef SHINKIRO_DEBUG
#        pragma comment( lib, "spdlogd" )
#    else
#        pragma comment( lib, "spdlog" )
#    endif
#    include <spdlog/spdlog.h>

#    include <spdlog/fmt/ostr.h>

#    include <spdlog/sinks/basic_file_sink.h>
#    include <spdlog/sinks/stdout_color_sinks.h>
#    pragma warning( pop )
#endif

#ifndef SHINKIRO_CORE_HEADERS
#    define SHINKIRO_CORE_HEADERS
#    include <Core/Util/Assert.h>
#    include <Core/Util/Log.h>
#    include <Core/Util/Macro.h>
#endif
