#include <Asset/Log.h>

#include <iostream>

namespace Shinkiro::Asset
{
    std::ofstream Log::m_LogFile;
    std::mutex    Log::m_Mutex;

    void Log::Init( const std::string & filename )
    {
        std::lock_guard<std::mutex> lock( m_Mutex );
        if ( m_LogFile.is_open() )
        {
            m_LogFile.close();
        }
        m_LogFile.open( filename, std::ios::out | std::ios::trunc );
        if ( !m_LogFile.is_open() )
        {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }

    void Log::Shutdown()
    {
        std::lock_guard<std::mutex> lock( m_Mutex );
        if ( m_LogFile.is_open() )
        {
            m_LogFile.close();
        }
    }

    void Log::Write( const std::string & message )
    {
        std::lock_guard<std::mutex> lock( m_Mutex );
        if ( m_LogFile.is_open() )
        {
            m_LogFile << message << std::endl;
        }
    }

}
