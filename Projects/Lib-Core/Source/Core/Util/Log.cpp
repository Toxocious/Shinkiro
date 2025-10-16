#include <Core/_Common.h>

#include <Core/Util/Log.h>

namespace Shinkiro::Logger
{
    Ref<Log> Log::s_CoreLogger;
    Ref<Log> Log::s_AppLogger;

    Ref<Log> & Log::GetCoreLogger()
    {
        return s_CoreLogger;
    }

    Ref<Log> & Log::GetAppLogger()
    {
        return s_AppLogger;
    }

    void Log::Initialize()
    {
#ifdef _DEBUG
        if ( !s_LogFile.is_open() )
        {
            // Create "Logs" directory if it doesn't exist
            std::filesystem::create_directories( "Logs" );

            // Get local time safely
            auto        now         = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t( now );
            std::tm     localTime {};

#    ifdef _WIN32
            localtime_s( &localTime, &currentTime );
#    else
            localtime_r( &currentTime, &localTime );
#    endif

            // Build timestamp string
            std::ostringstream oss;
            oss << std::put_time( &localTime, "%Y-%m-%d_%H-%M-%S" );
            std::string timestamp = oss.str();

            // Construct log file path
            std::string logPath = "Logs/Shinkiro_" + timestamp + ".log";

            // Open log file
            s_LogFile.open( logPath, std::ios::out | std::ios::app );
            if ( s_LogFile )
            {
                s_LogFile << "__| |______________________________________| |__\n";
                s_LogFile << " __   ______________________________________   __\n";
                s_LogFile << "   | | ____  _     _       _    _           | |\n";
                s_LogFile << "   | |/ ___|| |__ (_)_ __ | | _(_)_ __ ___  | |\n";
                s_LogFile << "   | |\___ \\| '_ \\| | '_ \\ | |/ / | '__/ _ \\ | |\n";
                s_LogFile << "   | | ___) | | | | | | | |   <| | | | (_) || |\n";
                s_LogFile << "   | ||____/|_| |_|_|_| |_|_|\\ _\\ _|_|\\ ___/ | |\n";
                s_LogFile << " __| |______________________________________| |__\n";
                s_LogFile << " __   ______________________________________   __\n";
                s_LogFile << "   | |        " << timestamp << "         | |\n\n";
                s_LogFile.flush();
            }
        }
#endif

        s_CoreLogger = CreateRef<Log>();
        s_AppLogger  = CreateRef<Log>();
    }
}
