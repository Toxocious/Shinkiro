#pragma once

#ifndef SHINKIRO_CORE_WINDOW_H
#    define SHINKIRO_CORE_WINDOW_H

#    include <Core/_Defs.h>

#    include <Core/_Common.h>

#    include <Core/Modules/_Module.h>

#    include <Asset/AssetBundleManager.h>

#    include <chrono>
#    include <string>

#    include <glm/glm.hpp>
#    include <stb_image/stb_image.h>

void SetKeyCallbacks( GLFWwindow * window, int key, int scancode, int action, int mods );

namespace Shinkiro::Core
{
    class CORE_API Window : public Module
    {
    public:
        Window( bool enabled = true );
        ~Window();

        bool Initialize( const char * title, int height, int width );
        bool Start() override;
        bool CleanUp() override;

        Shinkiro::Core::UpdateStatus PreUpdate() override;
        Shinkiro::Core::UpdateStatus Update() override;
        Shinkiro::Core::UpdateStatus PostUpdate() override;

        // Window utilities
        void SetTitle( const char * title );
        void SetSize( int width, int height );
        void SetPosition( int x, int y );
        void CenterWindow();

        // GLFW
        int  ShouldClose();
        void SwapBuffers();
        void PollEvents();

        // Callbacks
        static void SetMouseCallbacks( GLFWwindow * window, double xposIn, double yposIn );
        static void SetMouseWheelCallbacks( GLFWwindow * window, double xOffset, double yOffset );
        static void SetKeyCallbacks( GLFWwindow * window, int key, int scancode, int action, int mods );

        // Getters
        GLFWwindow * GetGLFWWindow()
        {
            return m_Window.get();
        }

        glm::ivec2 GetSize() const
        {
            return { m_Width, m_Height };
        }

        glm::ivec2 GetPosition() const
        {
            return { m_X, m_Y };
        }

        inline const float & GetDeltaTime() const
        {
            return m_DeltaTime;
        }

        inline const glm::vec2 & GetMousePos() const
        {
            return m_MousePos;
        }

        inline void SetMousePos( glm::vec2 MousePosition )
        {
            m_MousePos = MousePosition;
        }

    private:
        std::unique_ptr<GLFWwindow, decltype( &glfwDestroyWindow )> m_Window;

        stbi_uc * m_WindowIcon = nullptr;
        glm::vec2 m_MousePos   = glm::vec2( 0.0f, 0.0f );

        const char * m_Title;
        int          m_Width;
        int          m_Height;
        int          m_X { 0 };
        int          m_Y { 0 };

        float m_DeltaTime  = 0;
        float m_LastTime   = 0;
        int   m_FrameCount = 0;

    private:
        Shinkiro::Asset::AssetBundleManager m_BundleManager;

        std::unordered_map<std::string, std::vector<uint8_t>> m_Assets;
    };
}

#endif
