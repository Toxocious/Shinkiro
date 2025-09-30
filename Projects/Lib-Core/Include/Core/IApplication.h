#pragma once
#ifndef SHINKIRO_CORE_IAPPLICATION_H
#    define SHINKIRO_CORE_IAPPLICATION_H

#    include <Core/_Defs.h>

#    include <Core/Util/UpdateStatus.h>

#    include <Core/Interfaces/AudioManagerInterface.h>

#    include <string>

namespace Shinkiro::Asset
{
    class AssetBundleManager;
}

namespace Shinkiro::Renderer
{
    class MapManager;
    class Camera;
}

namespace Shinkiro::Platform
{
    class Window;
    class InputHandler;
}

namespace Shinkiro::Core
{
    enum class CORE_API AppState
    {
        LOADING,
        RUNNING
    };

    /**
     * @brief An abstracted interface for creating and managing applications.
     */
    class CORE_API IApplication
    {
    public:
        virtual ~IApplication() = default;

        /**
         * @brief Initializes a new application
         * @param title The window title
         * @param version The application version
         * @param width The window width
         * @param height The window height
         * @return True if initialization was successful
         */
        virtual bool         Initialize( const std::string & title, const std::string & version, int width, int height ) = 0;
        virtual bool         Start()                                                                                     = 0;
        virtual UpdateStatus Update()                                                                                    = 0;
        virtual bool         CleanUp()                                                                                   = 0;

    public:
        /**
         * @brief Returns a ref to the Bundle Manager
         */
        virtual Asset::AssetBundleManager & GetBundleManager() = 0;

        /**
         * @brief Returns a ref to the Audio Manager
         */
        virtual Audio::IAudioManager & GetAudioManager() = 0;

        /**
         * @brief Returns a ref to the Map Manager
         */
        virtual Renderer::MapManager & GetMapManager() = 0;

        /**
         * @brief Returns a ref to the Camera
         */
        virtual Renderer::Camera & GetCamera() = 0;

        /**
         * @brief Returns a ref to the InputHandler
         */
        virtual Platform::InputHandler * GetInputHandler() = 0;

        /**
         * @brief Returns a pointer to the active window
         */
        virtual Platform::Window * GetWindow() = 0;

    public:
        AppState m_AppState = AppState::LOADING;
    };

    /**
     * @brief Global pointer to the single application instance (interface type).
     */
    extern CORE_API IApplication * App;
}

#endif
