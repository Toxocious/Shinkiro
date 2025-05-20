#include <Core/_Defs.h>

#include <Core/Util/Log.h>

namespace Shinkiro::Core
{
    Ref<spdlog::logger> Log::s_CoreLogger;
    Ref<spdlog::logger> Log::s_ApplicationLogger;

    Ref<spdlog::logger> & Log::GetCoreLogger()
    {
        return s_CoreLogger;
    }

    Ref<spdlog::logger> & Log::GetApplicationLogger()
    {
        return s_ApplicationLogger;
    }

    void Log::Init()
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );
        logSinks[0]->set_pattern( "%^[%T] %n: %v%$" );

#ifdef SHINKIRO_DEBUG
        logSinks.emplace_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>( "Shinkiro.log", true ) );
        logSinks[1]->set_pattern( "[%T] [%l] %n: %v" );
#endif

        s_CoreLogger = std::make_shared<spdlog::logger>( "SHINKIRO", begin( logSinks ), end( logSinks ) );
        spdlog::register_logger( s_CoreLogger );
        s_CoreLogger->set_level( spdlog::level::trace );
        s_CoreLogger->flush_on( spdlog::level::trace );

        s_ApplicationLogger = std::make_shared<spdlog::logger>( "APPLICATION", begin( logSinks ), end( logSinks ) );
        spdlog::register_logger( s_ApplicationLogger );
        s_ApplicationLogger->set_level( spdlog::level::trace );
        s_ApplicationLogger->flush_on( spdlog::level::trace );
    }
}
