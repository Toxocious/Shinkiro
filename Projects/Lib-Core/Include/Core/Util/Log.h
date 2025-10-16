#pragma once

#ifndef SHINKIRO_CORE_LOG_H
#    define SHINKIRO_CORE_LOG_H

#    include <Core/_Defs.h>

#    include <Core/Util/Macro.h>

#    include <chrono>
#    include <ctime>
#    include <format>
#    include <fstream>
#    include <iomanip>
#    include <iostream>
#    include <mutex>
#    include <sstream>
#    include <string_view>

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

namespace Shinkiro::Logger
{
    enum class CORE_API LogSource
    {
        Core,
        Application
    };

    enum class CORE_API LogLevel
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    constexpr std::string_view ToString( LogLevel level ) noexcept
    {
        switch ( level )
        {
            case LogLevel::Trace:
                return "TRACE";
            case LogLevel::Debug:
                return "DEBUG";
            case LogLevel::Info:
                return "INFO";
            case LogLevel::Warn:
                return "WARN";
            case LogLevel::Error:
                return "ERROR";
            case LogLevel::Critical:
                return "CRITICAL";
            default:
                return "UNKNOWN";
        }
    }

    class CORE_API Log
    {
    public:
        static void Initialize();

        template <typename... Args>
        void WriteLog( LogSource source, LogLevel level, std::string_view fmt, Args &&... args )
        {
            const auto now  = std::chrono::system_clock::now();
            const auto time = std::chrono::system_clock::to_time_t( now );
            std::tm    tm {};

#    if defined( _WIN32 )
            localtime_s( &tm, &time );
#    else
            localtime_r( &time, &tm );
#    endif

            std::ostringstream oss;
            oss << std::put_time( &tm, "%H:%M:%S" );

            std::string formatted;
            if constexpr ( sizeof...( Args ) == 0 )
            {
                formatted = std::string( fmt );
            }
            else
            {
                formatted = std::vformat( fmt, std::make_format_args( args... ) );
            }

            std::lock_guard lock( m_Mutex );
            std::cout << "[" << oss.str() << "] "
                      << "[" << ( source == LogSource::Core ? "SHINKIRO" : "APPLICATION" ) << "] "
                      << "[" << ToString( level ) << "] "
                      << formatted << "\n";

#    ifdef _DEBUG
            if ( s_LogFile )
            {
                s_LogFile << "[" << oss.str() << "] "
                          << "[" << ( source == LogSource::Core ? "SHINKIRO" : "APPLICATION" ) << "] "
                          << "[" << ToString( level ) << "] "
                          << formatted << "\n";
                s_LogFile.flush();
            }
#    endif
        }

    public:
        static Ref<Log> & GetCoreLogger();
        static Ref<Log> & GetAppLogger();

    private:
        std::mutex m_Mutex;

        static Ref<Log> s_CoreLogger;
        static Ref<Log> s_AppLogger;

#    ifdef _DEBUG
        static inline std::ofstream s_LogFile;
#    endif
    };
}

#    define SHNK_CORE_TRACE( fmt, ... )    ::Shinkiro::Logger::Log::GetCoreLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Core, ::Shinkiro::Logger::LogLevel::Trace, fmt, __VA_ARGS__ )
#    define SHNK_CORE_DEBUG( fmt, ... )    ::Shinkiro::Logger::Log::GetCoreLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Core, ::Shinkiro::Logger::LogLevel::Debug, fmt, __VA_ARGS__ )
#    define SHNK_CORE_INFO( fmt, ... )     ::Shinkiro::Logger::Log::GetCoreLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Core, ::Shinkiro::Logger::LogLevel::Info, fmt, __VA_ARGS__ )
#    define SHNK_CORE_WARN( fmt, ... )     ::Shinkiro::Logger::Log::GetCoreLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Core, ::Shinkiro::Logger::LogLevel::Warn, fmt, __VA_ARGS__ )
#    define SHNK_CORE_ERROR( fmt, ... )    ::Shinkiro::Logger::Log::GetCoreLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Core, ::Shinkiro::Logger::LogLevel::Error, fmt, __VA_ARGS__ )
#    define SHNK_CORE_CRITICAL( fmt, ... ) ::Shinkiro::Logger::Log::GetCoreLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Core, ::Shinkiro::Logger::LogLevel::Critical, fmt, __VA_ARGS__ )

#    define SHNK_TRACE( fmt, ... )    ::Shinkiro::Logger::Log::GetAppLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Application, ::Shinkiro::Logger::LogLevel::Trace, fmt, __VA_ARGS__ )
#    define SHNK_DEBUG( fmt, ... )    ::Shinkiro::Logger::Log::GetAppLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Application, ::Shinkiro::Logger::LogLevel::Debug, fmt, __VA_ARGS__ )
#    define SHNK_INFO( fmt, ... )     ::Shinkiro::Logger::Log::GetAppLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Application, ::Shinkiro::Logger::LogLevel::Info, fmt, __VA_ARGS__ )
#    define SHNK_WARN( fmt, ... )     ::Shinkiro::Logger::Log::GetAppLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Application, ::Shinkiro::Logger::LogLevel::Warn, fmt, __VA_ARGS__ )
#    define SHNK_ERROR( fmt, ... )    ::Shinkiro::Logger::Log::GetAppLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Application, ::Shinkiro::Logger::LogLevel::Error, fmt, __VA_ARGS__ )
#    define SHNK_CRITICAL( fmt, ... ) ::Shinkiro::Logger::Log::GetAppLogger().get()->WriteLog( ::Shinkiro::Logger::LogSource::Application, ::Shinkiro::Logger::LogLevel::Critical, fmt, __VA_ARGS__ )

#endif
