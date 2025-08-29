#pragma once

#ifndef SHINKIRO_CORE_LOG_H
#    define SHINKIRO_CORE_LOG_H

#    include <Log/_Defs.h>

#    include <Log/Util/Macro.h>

#    include <spdlog/spdlog.h>

namespace Shinkiro::Logger
{
    class Log
    {
    public:
        LOG_API static void Initialize();

        LOG_API static Ref<spdlog::logger> & GetCoreLogger();
        LOG_API static Ref<spdlog::logger> & GetApplicationLogger();

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
