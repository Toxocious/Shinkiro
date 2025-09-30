#pragma once

#ifndef SHINKIRO_CORE_LOG_H
#    define SHINKIRO_CORE_LOG_H

#    include <Core/_Defs.h>

#    include <Core/Util/Macro.h>

#    if defined _WIN32
#        ifndef NOMINMAX
#            define NOMINMAX
#        endif
#        ifndef WIN32_LEAN_AND_MEAN
#            define WIN32_LEAN_AND_MEAN
#        endif

#        include <Windows.h>

#        if !defined( assert )
#            include <stdio.h>

#            define assert( x )                                      \
                do                                                   \
                {                                                    \
                    if ( !( x ) )                                    \
                    {                                                \
                        printf( "Error: assert(%s) failed!\n", #x ); \
                    }                                                \
                } while ( 0 )
#        endif
#    endif

#    ifndef SHINKIRO_IMPL_SPDLOG
#        define SHINKIRO_IMPL_SPDLOG
#        pragma warning( push, 0 )
#        ifdef SHINKIRO_DEBUG
#            pragma comment( lib, "spdlogd" )
#        else
#            pragma comment( lib, "spdlog" )
#        endif

#        include <spdlog/spdlog.h>

#        include <spdlog/fmt/ostr.h>

#        include <spdlog/sinks/basic_file_sink.h>
#        include <spdlog/sinks/stdout_color_sinks.h>
#        pragma warning( pop )
#    endif

namespace Shinkiro::Logger
{
    class Log
    {
    public:
        CORE_API static void Initialize();

        CORE_API static Ref<spdlog::logger> & GetCoreLogger();
        CORE_API static Ref<spdlog::logger> & GetApplicationLogger();

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ApplicationLogger;
    };
}

// DLL log macros
#    define SHNK_CORE_TRACE( ... )    ::Shinkiro::Logger::Log::GetCoreLogger()->trace( __VA_ARGS__ )
#    define SHNK_CORE_INFO( ... )     ::Shinkiro::Logger::Log::GetCoreLogger()->info( __VA_ARGS__ )
#    define SHNK_CORE_WARN( ... )     ::Shinkiro::Logger::Log::GetCoreLogger()->warn( __VA_ARGS__ )
#    define SHNK_CORE_ERROR( ... )    ::Shinkiro::Logger::Log::GetCoreLogger()->error( __VA_ARGS__ )
#    define SHNK_CORE_CRITICAL( ... ) ::Shinkiro::Logger::Log::GetCoreLogger()->critical( __VA_ARGS__ )

// Application log macros
#    define SHNK_TRACE( ... )    ::Shinkiro::Logger::Log::GetApplicationLogger()->trace( __VA_ARGS__ )
#    define SHNK_INFO( ... )     ::Shinkiro::Logger::Log::GetApplicationLogger()->info( __VA_ARGS__ )
#    define SHNK_WARN( ... )     ::Shinkiro::Logger::Log::GetApplicationLogger()->warn( __VA_ARGS__ )
#    define SHNK_ERROR( ... )    ::Shinkiro::Logger::Log::GetApplicationLogger()->error( __VA_ARGS__ )
#    define SHNK_CRITICAL( ... ) ::Shinkiro::Logger::Log::GetApplicationLogger()->critical( __VA_ARGS__ )

#endif
