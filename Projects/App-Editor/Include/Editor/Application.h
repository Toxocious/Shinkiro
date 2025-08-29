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
        /**
         * @brief Initializes the application with the specified parameters.
         * @param name The name of the application.
         * @param version The version of the application.
         * @param height The height of the application window.
         * @param width The width of the application window.
         */
        bool Initialize( const std::string & name, const std::string & version, int height, int width ) override;

        /**
         * @brief Starts the application, initializing all modules and entering the main loop.
         * @return True if the application started successfully, false otherwise.
         */
        bool Start() override;

        /**
         * @brief Cleans up resources and shuts down the application.
         * @return True if cleanup was successful, false otherwise.
         */
        bool CleanUp() override;

        /**
         * @brief Updates the application state, processing events and rendering.
         * @return The current update status (continue, stop, error).
         */
        Shinkiro::Core::UpdateStatus Update() override;

    public:
        /**
         * @brief Get the application's Bundle Manager
         * @return A reference to the Bundle Manager
         */
        Asset::AssetBundleManager & GetBundleManager() override;

        /**
         * @brief Get the application's Audio Manager
         * @return A reference to the Audio Manager
         */
        Audio::AudioManager &       GetAudioManager() override;

        /**
         * @brief Get the window module of the application
         * @return A pointer to the window module
         */
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
