#pragma once

#ifndef SHINKIRO_CORE_APPLICATION_H
#    define SHINKIRO_CORE_APPLICATION_H

#    include <Core/_Defs.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Core
{
    enum class CORE_API ApplicationState
    {
        EXIT_ERROR = -1,
        EXIT       = 0,

        CREATION,
        INIT,
        START,
        UPDATE,
        FINISH
    };

    class CORE_API Application
    {
    public:
        Application( std::string name, std::string version, int height, int width );
        ~Application();

    public:
        bool Initialize();
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

// extern Shinkiro::Core::Application * App;

#endif
