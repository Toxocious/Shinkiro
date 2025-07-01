#pragma once

#ifndef SHINKIRO_CORE_APPLICATION_H
#    define SHINKIRO_CORE_APPLICATION_H

#    include <Core/_Defs.h>

#    include <Core/Modules/_Module.h>

#    include <Core/Modules/Window.h>

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
        Application();
        ~Application();

    public:
        bool Initialize( std::string name, std::string version, int height, int width );
        bool Start();
        bool CleanUp();

        bool                         PreUpdate();
        Shinkiro::Core::UpdateStatus Update();
        bool                         PostUpdate();

    public:
        Shinkiro::Core::Window * m_Window = nullptr;

    private:
        std::vector<Module *> m_Modules;

    private:
        std::string m_Name;
        std::string m_Version;
        int         m_Height;
        int         m_Width;
    };

    extern Application * App;
}

#endif
