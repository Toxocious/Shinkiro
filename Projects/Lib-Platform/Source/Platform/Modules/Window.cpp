#include <Platform/_Common.h>

#include <Platform/Modules/Window.h>

#include <Platform/InputHandler.h>

#include <Core/IApplication.h>

#include <Asset/AssetBundleManager.h>
#include <Asset/_FilePath.h>

#include <lucide/IconsLucide.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <chrono>
#include <thread>

namespace Shinkiro::Platform
{
    Window::Window( bool enabled )
        : Module( "Window", enabled ), m_Window( nullptr, &glfwDestroyWindow )
    {
    }

    Window::~Window()
    {
        CleanUp();
    }

    bool Window::Initialize( const std::string & title, const std::string & version, const std::string build_type, int height, int width )
    {
        m_Height = height;
        m_Width  = width;
        m_Title  = title;

        if ( !glfwInit() )
        {
            SHNK_CORE_ERROR( "Failed to initialize GLFW" );
            return false;
        }

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
        glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

        m_Window.reset( glfwCreateWindow( m_Width, m_Height, m_Title.c_str(), nullptr, nullptr ) );
        if ( !m_Window )
        {
            SHNK_CORE_ERROR( "Failed to create the GLFW window" );
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent( GetGLFWWindow() );
        glfwSwapInterval( 0 );

        if ( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
        {
            SHNK_CORE_ERROR( "Failed to initialize GLAD" );
            return false;
        }

        glfwSetKeyCallback( GetGLFWWindow(), SetKeyCallbacks );
        glfwSetCursorPosCallback( GetGLFWWindow(), SetMouseCallbacks );
        glfwSetScrollCallback( GetGLFWWindow(), SetMouseWheelCallbacks );
        glfwSetMouseButtonCallback( GetGLFWWindow(), SetMouseButtonCallbacks );
        glfwSetInputMode( GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );

        glEnable( GL_DEPTH_TEST );
        glCullFace( GL_BACK );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        CenterWindow();
        SetWindowIcon();
        LoadLogo();

        // if ( !InitializeImGui() )
        // {
        //     SHNK_CORE_ERROR( "Failed to initialize ImGui Layer" );
        //     return false;
        // }

        return true;
    }

    bool Window::CleanUp()
    {
        if ( m_GuiLayer )
        {
            m_GuiLayer->Shutdown();
            delete m_GuiLayer;
            m_GuiLayer = nullptr;
        }

        if ( m_Window )
        {
            m_Window.reset();
            glfwTerminate();
        }

        return true;
    }

    bool Window::Start()
    {
        return true;
    }

    Shinkiro::Core::UpdateStatus Window::PreUpdate()
    {
        PollEvents();

        Shinkiro::Platform::InputHandler::Get().Update();

        float currentTime = ( float ) glfwGetTime();
        m_DeltaTime       = currentTime - m_LastTime;
        m_LastTime        = currentTime;
        m_FrameCount++;

        double xpos, ypos;
        glfwGetCursorPos( GetGLFWWindow(), &xpos, &ypos );
        SetMousePos( glm::vec2( xpos, ypos ) );

        if ( m_InputCallback )
        {
            m_InputCallback( m_DeltaTime );
        }

        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    Shinkiro::Core::UpdateStatus Window::Update()
    {
        if ( ShouldClose() )
        {
            return Shinkiro::Core::UpdateStatus::UPDATE_STOP;
        }

        Render();

        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    Shinkiro::Core::UpdateStatus Window::PostUpdate()
    {
        SwapBuffers();

        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    void Window::Render()
    {
        if ( m_RenderCallback )
        {
            m_RenderCallback();
        }
    }

    bool Window::InitializeImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO & io    = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.Fonts->AddFontDefault();

        ImFontConfig config;
        config.MergeMode                   = true;
        config.GlyphMinAdvanceX            = 13.0f; // monospaced font
        static const ImWchar icon_ranges[] = { ICON_MIN_LC, ICON_MAX_LC, 0 };

        const auto LucideFont = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Fonts/Lucide/lucide.ttf" );

        if ( !LucideFont.empty() )
        {
            // ImGui takes ownership of the font data and will free() it.
            // We must allocate a copy using malloc because the vector will free its own memory.
            int    fontDataSize = static_cast<int>( LucideFont.size() );
            void * fontDataCopy = malloc( fontDataSize );

            if ( fontDataCopy )
            {
                memcpy( fontDataCopy, LucideFont.data(), fontDataSize );
                io.Fonts->AddFontFromMemoryTTF( fontDataCopy, fontDataSize, 22.0f, &config, icon_ranges );
            }
        }

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL( GetGLFWWindow(), true );
        ImGui_ImplOpenGL3_Init( "#version 330" );

        SHNK_CORE_INFO( "Dear ImGui initialized successfully (Platform Backend)." );

        if ( m_GuiLayer )
        {
            return m_GuiLayer->Initialize( ImGui::GetCurrentContext() );
        }

        return true;
    }

    bool Window::ShutdownImGui()
    {
        // Shutdown GUI Layer first (release textures, etc.)
        if ( m_GuiLayer )
        {
            m_GuiLayer->Shutdown();
        }

        // Shutdown Backends
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        return true;
    }

    void Window::BeginImGuiFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if ( m_GuiLayer )
        {
            m_GuiLayer->BeginFrame();
        }
    }

    void Window::EndImGuiFrame()
    {
        if ( m_GuiLayer )
        {
            m_GuiLayer->EndFrame();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
    }

    void Window::RenderImGui()
    {
        if ( m_GuiLayer )
        {
            m_GuiLayer->RenderPanels();
        }
    }

    // ================================================================================
    // ================================================================================
    // ================================================================================

    void Window::SetTitle( const char * title )
    {
        m_Title = title;
        if ( m_Window )
        {
            glfwSetWindowTitle( m_Window.get(), m_Title.c_str() );
        }
    }

    void Window::SetSize( int width, int height )
    {
        m_Width  = width;
        m_Height = height;
        if ( m_Window )
        {
            glfwSetWindowSize( m_Window.get(), m_Width, m_Height );
        }
    }

    void Window::SetPosition( int x, int y )
    {
        m_X = x;
        m_Y = y;
        if ( m_Window )
        {
            glfwSetWindowPos( m_Window.get(), m_X, m_Y );
        }
    }

    int Window::ShouldClose()
    {
        return glfwWindowShouldClose( GetGLFWWindow() );
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers( GetGLFWWindow() );
    }

    void Window::PollEvents()
    {
        return glfwPollEvents();
    }

    void Window::SetMouseCallbacks( GLFWwindow * window, double xposIn, double yposIn )
    {
    }

    void Window::SetMouseWheelCallbacks( GLFWwindow * window, double xOffset, double yOffset )
    {
    }

    void Window::SetMouseButtonCallbacks( GLFWwindow * window, int button, int action, int mods )
    {
        InputHandler::Get().MouseButtonCallback( button, action, mods );
    }

    void Window::SetKeyCallbacks( GLFWwindow * window, int key, int scancode, int action, int mods )
    {
        if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        {
            glfwSetWindowShouldClose( window, GLFW_TRUE );
        }

        InputHandler::Get().KeyCallback( key, scancode, action, mods );
    }

    void Window::LoadLogo()
    {
        const auto ShinkiroLogo = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Textures/Shinkiro/LogoTest.png" );

        int logoWidth, logoHeight, channels;

        unsigned char * m_ShinkiroLogo = stbi_load_from_memory(
            ShinkiroLogo.data(),
            static_cast<int>( ShinkiroLogo.size() ),
            &logoWidth,
            &logoHeight,
            &channels,
            4
        );

        if ( m_ShinkiroLogo != nullptr )
        {
            m_LogoHeight = logoHeight / 4;
            m_LogoWidth  = logoWidth / 4;

            unsigned char * resizedLogo = new unsigned char[m_LogoWidth * m_LogoHeight * 4];

            stbir_resize_uint8(
                m_ShinkiroLogo,
                logoWidth,
                logoHeight,
                0,
                resizedLogo,
                m_LogoWidth,
                m_LogoHeight,
                0,
                4
            );

            glGenTextures( 1, &m_LogoTextureID );
            glBindTexture( GL_TEXTURE_2D, m_LogoTextureID );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_LogoWidth, m_LogoHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, resizedLogo );
            glGenerateMipmap( GL_TEXTURE_2D );

            delete[] resizedLogo;
            stbi_image_free( m_ShinkiroLogo );
        }
        else
        {
            SHNK_CORE_ERROR( "Failed to load logo from memory: {0}", stbi_failure_reason() );
        }
    }

    void Window::SetWindowIcon()
    {
        const auto ShinkiroWindowIcon = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Textures/Shinkiro/LogoMiniSmall.png" );

        int iconWidth, iconHeight, channels;

        unsigned char * m_WindowIcon = stbi_load_from_memory(
            ShinkiroWindowIcon.data(),
            static_cast<int>( ShinkiroWindowIcon.size() ),
            &iconWidth,
            &iconHeight,
            &channels,
            4
        );

        if ( m_WindowIcon != nullptr )
        {
            m_IconHeight = iconHeight;
            m_IconWidth  = iconWidth;

            GLFWimage images[1];
            images[0].width  = iconWidth;
            images[0].height = iconHeight;
            images[0].pixels = m_WindowIcon;

            glGenTextures( 1, &m_IconTextureID );
            glBindTexture( GL_TEXTURE_2D, m_IconTextureID );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_IconWidth, m_IconHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_WindowIcon );
            glGenerateMipmap( GL_TEXTURE_2D );

            glfwSetWindowIcon( GetGLFWWindow(), 1, images );

            stbi_image_free( m_WindowIcon );
        }
        else
        {
            SHNK_CORE_ERROR( "Failed to load icon from memory: {0}", stbi_failure_reason() );
        }
    }

    void Window::CenterWindow()
    {
        int window_width, window_height;
        glfwGetWindowSize( GetGLFWWindow(), &window_width, &window_height );

        int            monitor_count;
        GLFWmonitor ** monitors = glfwGetMonitors( &monitor_count );
        if ( !monitors || monitor_count == 0 )
        {
            return;
        }

        GLFWmonitor * monitor = glfwGetPrimaryMonitor();
        if ( !monitor )
        {
            return;
        }

        int monitor_x, monitor_y;
        glfwGetMonitorPos( monitor, &monitor_x, &monitor_y );

        const GLFWvidmode * mode = glfwGetVideoMode( monitor );
        if ( !mode )
        {
            return;
        }

        int center_x = monitor_x + ( mode->width - window_width ) / 2;
        int center_y = monitor_y + ( mode->height - window_height ) / 2;

        m_X = center_x;
        m_Y = center_y;

        glfwSetWindowPos( GetGLFWWindow(), center_x, center_y );
    }
}
