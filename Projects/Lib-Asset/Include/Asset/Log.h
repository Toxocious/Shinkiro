#pragma once

#include <Asset/_Defs.h>

#include <fstream>
#include <mutex>

namespace Shinkiro::Asset
{
    class ASSET_API Log
    {
    public:
        static void Init( const std::string & filename );

        static void Shutdown();

        static void Write( const std::string & message );

    private:
        static std::ofstream m_LogFile;
        static std::mutex    m_Mutex;
    };
}
