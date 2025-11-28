#pragma once

#ifndef SHINKIRO_EDITOR_APPLICATION_H
#    define SHINKIRO_EDITOR_APPLICATION_H

#    include <Core/_Defs.h>

#    include <Core/IApplication.h>
#    include <Core/Interfaces/GuiLayerInterface.h>

#    include <Core/Util/UpdateStatus.h>

#    include <Asset/AssetBundleManagerFactory.h>

#    include <Audio/AudioManagerFactory.h>

#    include <Renderer/Map/MapManager.h>

#    include <Platform/Modules/Window.h>

#    include <string>
#    include <vector>

namespace Shinkiro::Platform
{
    class Module;
    class Window;
    class InputHandler;
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
        Shinkiro::Asset::IAssetBundleManager & GetBundleManager() override;

        /**
         * @brief Get the application's Audio Manager
         * @return A reference to the Audio Manager
         */
        // Shinkiro::Audio::AudioManager & GetAudioManager() override;
        Shinkiro::Audio::IAudioManager & GetAudioManager() override;

        /**
         * @brief Get the application's Map Renderer Manager
         * @return A reference to the Map Renderer Manager
         */
        Shinkiro::Renderer::MapManager & GetMapManager() override;

        /**
         * @brief Get the application's Camera
         * @return A reference to the Camera
         */
        Shinkiro::Renderer::Camera & GetCamera() override;

        /**
         * @brief Get the application's InputHandler
         * @return A reference to the InputHandler
         */
        Shinkiro::Platform::InputHandler * GetInputHandler() override;

        /**
         * @brief Get the window module of the application
         * @return A pointer to the window module
         */
        Shinkiro::Platform::Window * GetWindow() override;

    private:
        /**
         * @brief Sets input callbacks for the application.
         */
        void SetInputCallbacks();

        /**
         * @brief Sets render callbacks for the application.
         */
        void SetRenderCallbacks();

    public:
        Shinkiro::Renderer::MapManager m_MapManager;

        std::unique_ptr<Shinkiro::Asset::IAssetBundleManager> m_BundleManager;

        std::unique_ptr<Shinkiro::Audio::IAudioManager> m_AudioManager;

        Shinkiro::Renderer::Camera m_Camera;

    public:
        Shinkiro::Platform::InputHandler * m_InputHandler = nullptr;
        Shinkiro::Platform::Window *       m_Window       = nullptr;
        Shinkiro::Core::IGuiLayer *        m_GuiLayer     = nullptr;

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
