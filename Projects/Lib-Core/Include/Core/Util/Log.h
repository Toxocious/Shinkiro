#pragma once

#ifndef SHINKIRO_CORE_LOG_H
#    define SHINKIRO_CORE_LOG_H

#    include <Core/_Common.h>
#    include <Core/_Defs.h>

#    include <Core/Util/Assert.h>
#    include <Core/Util/Macro.h>

#    ifndef SHINKIRO_IMPL_SPDLOG
#        define SHINKIRO_IMPL_SPDLOG
#        pragma warning( push, 0 )
#        ifdef SHINKIRO_DEBUG
#            pragma comment( lib, "spdlogd" )
#        else
#            pragma comment( lib, "spdlog" )
#        endif
#        pragma warning( push, 0 )
#        pragma comment( lib, "spdlog" )
#        include <spdlog/spdlog.h>

#        include <spdlog/fmt/ostr.h>

#        include <spdlog/sinks/basic_file_sink.h>
#        include <spdlog/sinks/stdout_color_sinks.h>
#        pragma warning( pop )
#    endif

namespace Shinkiro::Core
{
    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger> & GetCoreLogger()
        {
            return s_CoreLogger;
        }

        static Ref<spdlog::logger> & GetEditorLogger()
        {
            return s_EditorLogger;
        }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_EditorLogger;
    };
}

// Core log macros
#    define SHNK_CORE_TRACE( ... )    ::Shinkiro::Log::GetCoreLogger()->trace( __VA_ARGS__ )
#    define SHNK_CORE_INFO( ... )     ::Shinkiro::Log::GetCoreLogger()->info( __VA_ARGS__ )
#    define SHNK_CORE_WARN( ... )     ::Shinkiro::Log::GetCoreLogger()->warn( __VA_ARGS__ )
#    define SHNK_CORE_ERROR( ... )    ::Shinkiro::Log::GetCoreLogger()->error( __VA_ARGS__ )
#    define SHNK_CORE_CRITICAL( ... ) ::Shinkiro::Log::GetCoreLogger()->critical( __VA_ARGS__ )

// Editor log macros
#    define SHNK_TRACE( ... )    ::Shinkiro::Log::GetEditorLogger()->trace( __VA_ARGS__ )
#    define SHNK_INFO( ... )     ::Shinkiro::Log::GetEditorLogger()->info( __VA_ARGS__ )
#    define SHNK_WARN( ... )     ::Shinkiro::Log::GetEditorLogger()->warn( __VA_ARGS__ )
#    define SHNK_ERROR( ... )    ::Shinkiro::Log::GetEditorLogger()->error( __VA_ARGS__ )
#    define SHNK_CRITICAL( ... ) ::Shinkiro::Log::GetEditorLogger()->critical( __VA_ARGS__ )

#endif
