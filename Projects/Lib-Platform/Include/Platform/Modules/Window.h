#pragma once

#ifndef SHINKIRO_PLATFORM_WINDOW_H
#    define SHINKIRO_PLATFORM_WINDOW_H

#    include <Platform/_Defs.h>

#    include <Platform/Modules/_Module.h>

#    include <Core/Util/UpdateStatus.h>

#    include <Core/Interfaces/GuiLayerInterface.h>

#    include <glad/glad.h>

#    include <glm/glm.hpp>

#    include <stb_image/stb_image.h>

#    include <functional>

void SetKeyCallbacks( GLFWwindow * window, int key, int scancode, int action, int mods );

namespace Shinkiro::Platform
{
    class PLATFORM_API Window : public Module
    {
    public:
        Window( bool enabled = true );
        ~Window();

        bool Initialize( const std::string & title, const std::string & version, const std::string build_type, int height, int width );
        bool Start() override;
        bool CleanUp() override;

        Shinkiro::Core::UpdateStatus PreUpdate() override;
        Shinkiro::Core::UpdateStatus Update() override;
        Shinkiro::Core::UpdateStatus PostUpdate() override;

        // Window
        void Render();

        // Window utilities
        void SetTitle( const char * title );
        void SetSize( int width, int height );
        void SetPosition( int x, int y );
        void LoadLogo();
        void SetWindowIcon();
        void CenterWindow();

        // GLFW
        int  ShouldClose();
        void SwapBuffers();
        void PollEvents();

        // ImGui
        void BeginImGuiFrame();
        void EndImGuiFrame();
        void RenderImGui();
        bool ShutdownImGui() override;
        bool InitializeImGui() override;

        // Callbacks
        static void SetMouseCallbacks( GLFWwindow * window, double xposIn, double yposIn );
        static void SetMouseWheelCallbacks( GLFWwindow * window, double xOffset, double yOffset );
        static void SetMouseButtonCallbacks( GLFWwindow * window, int button, int action, int mods );
        static void SetKeyCallbacks( GLFWwindow * window, int key, int scancode, int action, int mods );

        // Getters and Setters
        GLFWwindow * GetGLFWWindow()
        {
            return m_Window.get();
        }

        void SetGuiLayer( Shinkiro::Core::IGuiLayer * guiLayer )
        {
            m_GuiLayer = guiLayer;
        }

        inline const int GetHeight() const
        {
            return m_Height;
        }

        inline const int GetWidth() const
        {
            return m_Width;
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

        inline glm::vec2 GetLastMousePos() const
        {
            return m_LastMousePos;
        }

        inline bool IsFirstMouse() const
        {
            return m_FirstMouse;
        }

        inline void SetFirstMouse( bool first )
        {
            m_FirstMouse = first;
        }

        inline void SetLastMousePos( glm::vec2 pos )
        {
            m_LastMousePos = pos;
        }

        inline void SetRenderCallback( const std::function<void()> & callback )
        {
            m_RenderCallback = callback;
        }

        inline void SetInputCallback( const std::function<void( float )> & callback )
        {
            m_InputCallback = callback;
        }

    private:
        std::unique_ptr<GLFWwindow, decltype( &glfwDestroyWindow )> m_Window;

        Shinkiro::Core::IGuiLayer * m_GuiLayer = nullptr;

        std::function<void()>        m_RenderCallback;
        std::function<void( float )> m_InputCallback;

        glm::vec2 m_MousePos     = glm::vec2( 0.0f, 0.0f );
        glm::vec2 m_LastMousePos = glm::vec2( 0.0f, 0.0f );
        bool      m_FirstMouse   = true;

        std::string m_Title;
        int         m_Width;
        int         m_Height;
        int         m_X { 0 };
        int         m_Y { 0 };

        stbi_uc * m_ShinkiroLogo  = nullptr;
        GLuint    m_LogoTextureID = 0;
        int       m_LogoWidth     = 0;
        int       m_LogoHeight    = 0;

        stbi_uc * m_WindowIcon    = nullptr;
        GLuint    m_IconTextureID = 0;
        int       m_IconWidth     = 0;
        int       m_IconHeight    = 0;

        float m_DeltaTime  = 0;
        float m_LastTime   = 0;
        int   m_FrameCount = 0;
    };
}

#endif
