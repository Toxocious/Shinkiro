#pragma once

#ifndef SHINKIRO_CORE_APPLICATION_H
#    define SHINKIRO_CORE_APPLICATION_H

#    include <Core/_Defs.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Core
{
    class CORE_API Application
    {
    public:
        Application();
        ~Application();

    public:
        bool Initialize( std::string name, std::string version, int height, int width );
        bool Start();
        bool CleanUp();

        bool PreUpdate();
        bool Update();
        bool PostUpdate();

    private:
        std::string m_Name;
        std::string m_Version;
        int         m_Height;
        int         m_Width;
    };

    extern Application * App;
}

#endif
