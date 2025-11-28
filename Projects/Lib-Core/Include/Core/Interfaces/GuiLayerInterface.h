#pragma once
#ifndef SHINKIRO_CORE_IGUILAYER_H
#    define SHINKIRO_CORE_IGUILAYER_H

#    include <Core/_Defs.h>

struct GLFWwindow;

namespace Shinkiro::Core
{
    class IGuiLayer
    {
    public:
        IGuiLayer()          = default;
        virtual ~IGuiLayer() = default;

        /**
         * @brief Initialize the GUI layer (Context, Backends, etc.)
         * @param windowHandle Pointer to the native window handle (e.g. GLFWwindow*)
         */
        virtual bool Initialize( void * imguiContext ) = 0;

        /**
         * @brief Shutdown the GUI layer
         */
        virtual void Shutdown() = 0;

        /**
         * @brief Begin a new frame
         */
        virtual void BeginFrame() = 0;

        /**
         * @brief End the current frame and render draw data
         */
        virtual void EndFrame() = 0;

        /**
         * @brief Render the GUI contents
         */
        virtual void Render() = 0;

        /**
         * @brief Render gui panels.
         */
        virtual void RenderLoadingScreen() = 0;

        /**
         * @brief Render gui panels.
         */
        virtual void RenderPanels() = 0;
    };
}

#endif
