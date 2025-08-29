#pragma once

#include <Asset/_Defs.h>

#include <fstream>
#include <mutex>

namespace Shinkiro::Asset
{
    class ASSET_API Log
    {
    public:
        /**
         * @brief Initializes the log system with the specified log file.
         * @param filename The name of the log file to create or overwrite.
         */
        static void Init( const std::string & filename );

        /**
         * @brief Shuts down the log system and closes the log file.
         */
        static void Shutdown();

        /**
         * @brief Writes a message to the log file.
         * @param message The message to log.
         */
        static void Write( const std::string & message );

    private:
        static std::ofstream m_LogFile;
        static std::mutex    m_Mutex;
    };
}
