#pragma once

#ifndef SHINKIRO_PLATFORM_APPLICATION_H
#    define SHINKIRO_PLATFORM_APPLICATION_H

#    include <Platform/_Defs.h>

#    include <Platform/Modules/_Module.h>

#    include <Platform/Modules/Window.h>

#    include <Asset/AssetBundleManager.h>

#    include <Audio/AudioManager.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Platform
{
    enum class PLATFORM_API ApplicationState
    {
        EXIT_ERROR = -1,
        EXIT       = 0,

        CREATION,
        INIT,
        START,
        UPDATE,
        FINISH
    };

    class PLATFORM_API Application
    {
    public:
        Application();
        ~Application();

    public:
        bool Initialize( std::string name, std::string version, int height, int width );
        bool Start();
        bool CleanUp();

        Shinkiro::Platform::UpdateStatus Update();

    public:
        Shinkiro::Asset::AssetBundleManager m_BundleManager;
        Audio::AudioManager                 m_AudioManager;

    public:
        Shinkiro::Platform::Window * m_Window = nullptr;

    private:
        std::vector<Shinkiro::Platform::Module *> m_Modules;

    private:
        std::string m_Name;
        std::string m_Version;
        int         m_Height;
        int         m_Width;
    };
}

extern PLATFORM_API Shinkiro::Platform::Application * App;

#endif
