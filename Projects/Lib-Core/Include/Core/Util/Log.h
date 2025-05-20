#pragma once

#ifndef SHINKIRO_CORE_LOG_H
#    define SHINKIRO_CORE_LOG_H

#    include <Core/_Defs.h>

#    include <Core/_Common.h>

#    include <Core/Util/Assert.h>
#    include <Core/Util/Macro.h>

namespace Shinkiro::Core
{
    class CORE_API Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger> & GetCoreLogger();
        static Ref<spdlog::logger> & GetApplicationLogger();

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ApplicationLogger;
    };
}

// Core log macros
#    define SHNK_CORE_TRACE( ... )    ::Shinkiro::Core::Log::GetCoreLogger()->trace( __VA_ARGS__ )
#    define SHNK_CORE_INFO( ... )     ::Shinkiro::Core::Log::GetCoreLogger()->info( __VA_ARGS__ )
#    define SHNK_CORE_WARN( ... )     ::Shinkiro::Core::Log::GetCoreLogger()->warn( __VA_ARGS__ )
#    define SHNK_CORE_ERROR( ... )    ::Shinkiro::Core::Log::GetCoreLogger()->error( __VA_ARGS__ )
#    define SHNK_CORE_CRITICAL( ... ) ::Shinkiro::Core::Log::GetCoreLogger()->critical( __VA_ARGS__ )

// Editor log macros
#    define SHNK_TRACE( ... )    ::Shinkiro::Core::Log::GetApplicationLogger()->trace( __VA_ARGS__ )
#    define SHNK_INFO( ... )     ::Shinkiro::Core::Log::GetApplicationLogger()->info( __VA_ARGS__ )
#    define SHNK_WARN( ... )     ::Shinkiro::Core::Log::GetApplicationLogger()->warn( __VA_ARGS__ )
#    define SHNK_ERROR( ... )    ::Shinkiro::Core::Log::GetApplicationLogger()->error( __VA_ARGS__ )
#    define SHNK_CRITICAL( ... ) ::Shinkiro::Core::Log::GetApplicationLogger()->critical( __VA_ARGS__ )

#endif
