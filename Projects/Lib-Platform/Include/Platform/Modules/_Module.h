#pragma once
#ifndef SHINKIRO_PLATFORM_MODULE_H
#    define SHINKIRO_PLATFORM_MODULE_H

#    include <Platform/_Defs.h>

#    include <Platform/Modules/_Module.h>

#    include <Core/Util/UpdateStatus.h>

#    include <string>

namespace Shinkiro::Platform
{
    class PLATFORM_API Module
    {
    public:
        Module( const char * name, bool enabled );
        virtual ~Module();

        /**
         * @brief Initialize the module. This is called once when the application starts
         */
        virtual bool Initialize();

        /**
         * @brief Initialize - specifically, the Window module - when the application starts.
         * @param title The title of the window
         * @param version The version of the application
         * @param build_type The build type (e.g., Debug, Release)
         * @param height The height of the window
         * @param width The width of the window
         */
        virtual bool Initialize( const std::string & title, const std::string & version, const std::string build_type, int height, int width );

        /**
         * @brief Start the module. This is called once when the application starts, after Initialize
         * @return True if the module started successfully, false otherwise
         */
        virtual bool Start();

        /**
         * @brief Clean up the module. This is called once when the application is closing
         * @return True if the module cleaned up successfully, false otherwise
         */
        virtual bool CleanUp();

        /**
         * @brief Initialize the ImGui context of the module if it exists. Not all modules will have ImGui contexts.
         */
        virtual bool InitializeImGui();

        /**
         * @brief Shutdown the ImGui context of the module if it exists. Not all modules will have ImGui contexts.
         */
        virtual bool ShutdownImGui();

        /**
         * @brief Any pre-update logic for the module. This is called once per frame, before Update()
         */
        virtual Shinkiro::Core::UpdateStatus PreUpdate() = 0;

        /**
         * @brief Any update logic for the module. This is called once per frame, after PreUpdate() and before PostUpdate()
         */
        virtual Shinkiro::Core::UpdateStatus Update() = 0;

        /**
         * @brief Any post-update logic for the module. This is called once per frame, after Update()
         */
        virtual Shinkiro::Core::UpdateStatus PostUpdate() = 0;

        /**
         * @brief Convert an UpdateStatus enum to a string
         */
        static std::string UpdateStatusToString( Shinkiro::Core::UpdateStatus us );

    public:
        std::string m_Name;

    private:
        bool m_Enabled = true;
    };
}

#endif
