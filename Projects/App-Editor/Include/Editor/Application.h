#pragma once

#ifndef SHINKIRO_EDITOR_APPLICATION_H
#    define SHINKIRO_EDITOR_APPLICATION_H

#    include <Core/_Defs.h>

#    include <Core/IApplication.h>

#    include <Core/Util/UpdateStatus.h>

#    include <Asset/AssetBundleManager.h>

#    include <Audio/AudioManager.h>

#    include <Platform/Modules/Window.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Platform
{
    class Module;
    class Window;
}

namespace Shinkiro
{
    enum class ApplicationState
    {
        EXIT_ERROR = -1,
        EXIT       = 0,

        CREATION,
        INIT,
        START,
        UPDATE,
        FINISH
    };

    class Application final : public Shinkiro::Core::IApplication
    {
    public:
        Application();
        ~Application();

    public:
        bool Initialize( const std::string & name, const std::string & version, int height, int width ) override;
        bool Start() override;
        bool CleanUp() override;

        Shinkiro::Core::UpdateStatus Update() override;

    public:
        Asset::AssetBundleManager & GetBundleManager() override;
        Audio::AudioManager &       GetAudioManager() override;
        Platform::Window *          GetWindow() override;

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

extern __declspec( dllexport ) Shinkiro::Application * App;

#endif
