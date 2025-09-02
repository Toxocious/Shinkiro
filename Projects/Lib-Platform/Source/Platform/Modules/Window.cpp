#include <Platform/_Common.h>

#include <Platform/Modules/Window.h>

#include <Core/IApplication.h>

#include <Asset/AssetBundleManager.h>

// #include <Renderer/Camera/Camera.h>
// #include <Renderer/Map/MapManager.h>

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
        glfwSetInputMode( GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );

        glEnable( GL_DEPTH_TEST );
        glCullFace( GL_BACK );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        CenterWindow();
        SetWindowIcon();
        LoadLogo();

        InitializeImGui();

        {
            // Shinkiro::Core::App->GetMapManager().LoadMap( "Maps/DecorTest.tmx" );
            // Shinkiro::Core::App->GetMapManager().SetActiveMap( "Maps/DecorTest.tmx" );
            // if ( GetMapManager().LoadMap( "Maps/DecorTest.tmx" ) )
            // {
            // GetMapManager().SetActiveMap( "Maps/DecorTest.tmx" );
            // }
        }

        return true;
    }

    bool Window::CleanUp()
    {
        glfwDestroyWindow( GetGLFWWindow() );
        glfwTerminate();

        return true;
    }

    bool Window::Start()
    {
        return true;
    }

    Shinkiro::Core::UpdateStatus Window::PreUpdate()
    {
        float currentTime = ( float ) glfwGetTime();
        m_DeltaTime       = currentTime - m_LastTime;
        m_LastTime        = currentTime;
        m_FrameCount++;

        double xpos, ypos;
        glfwGetCursorPos( GetGLFWWindow(), &xpos, &ypos );
        SetMousePos( glm::vec2( xpos, ypos ) );

        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    Shinkiro::Core::UpdateStatus Window::Update()
    {
        if ( ShouldClose() )
        {
            return Shinkiro::Core::UpdateStatus::UPDATE_STOP;
        }

        switch ( Shinkiro::Core::App->m_AppState )
        {
            case Core::AppState::LOADING:
                {
                    BeginImGuiFrame();
                    glClearColor( 0.169f, 0.169f, 0.169f, 1.0f );
                    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                    {
                        RenderImGui();
                    }

                    EndImGuiFrame();
                    SwapBuffers();

                    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

                    Shinkiro::Core::App->m_AppState = Core::AppState::RUNNING;

                    break;
                }

            case Core::AppState::RUNNING:
                {
                    Render();
                    break;
                }
        }

        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    Shinkiro::Core::UpdateStatus Window::PostUpdate()
    {
        SwapBuffers();
        PollEvents();

        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    void Window::Render()
    {
        if ( m_RenderCallback )
        {
            m_RenderCallback();
        }

        // glClearColor( 0.169f, 0.169f, 0.169f, 1.0f );
        // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // auto & mapManager = Shinkiro::Core::App->GetMapManager();
        // auto & camera     = Shinkiro::Core::App->GetCamera();

        // mapManager.Render( camera, m_Width, m_Height );
    }

    void Window::RenderImGui()
    {
        {
            ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );
            ImGui::SetNextWindowSize( ImGui::GetIO().DisplaySize );
            ImGui::Begin( "LoadingScreen", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground );

            float windowWidth  = ImGui::GetWindowSize().x;
            float windowHeight = ImGui::GetWindowSize().y;

            // Calculate total height of all elements for vertical centering
            float titleTextHeight    = ImGui::CalcTextSize( m_Title.c_str() ).y;
            float loadingTextHeight  = ImGui::CalcTextSize( "Loading..." ).y;
            float totalContentHeight = m_LogoHeight + titleTextHeight + loadingTextHeight + ImGui::GetStyle().ItemSpacing.y * 2;

            ImGui::SetCursorPosY( ( windowHeight - totalContentHeight ) * 0.5f );

            if ( m_LogoTextureID != 0 )
            {
                ImGui::SetCursorPosX( ( windowWidth - m_LogoWidth ) * 0.5f );
                ImGui::Image( ( intptr_t ) m_LogoTextureID, ImVec2( ( float ) m_LogoWidth, ( float ) m_LogoHeight ) );
            }

            float loadingTextWidth = ImGui::CalcTextSize( "Loading..." ).x;
            ImGui::SetCursorPosX( ( windowWidth - loadingTextWidth ) * 0.5f );
            ImGui::Text( "Loading..." );

            ImGui::End();
        }

        // if ( m_assetsLoaded )
        // {
        //     loadInitialAssets();

        //     loadSelectableObjects();
        //     // loadModels();

        //     m_state = AppState::RUNNING;
        // }
        // else
        // {
        //     m_assetsLoaded = true;
        // }
    }

    bool Window::InitializeImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO & io    = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGuiStyle & style = ImGui::GetStyle();

        ImGui_ImplGlfw_InitForOpenGL( GetGLFWWindow(), true );
        ImGui_ImplOpenGL3_Init( "#version 330" );

        SHNK_CORE_INFO( "Dear ImGui initialized successfully." );

        return true;
    }

    bool Window::ShutdownImGui()
    {
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
    }

    void Window::EndImGuiFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
    }

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

    void Window::SetMouseWheelCallbacks( GLFWwindow * window, double xOffset, double yOffset )
    {
    }

    void Window::SetMouseCallbacks( GLFWwindow * window, double xposIn, double yposIn )
    {
    }

    void Window::SetKeyCallbacks( GLFWwindow * window, int key, int scancode, int action, int mods )
    {
        if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        {
            glfwSetWindowShouldClose( window, GLFW_TRUE );
        }
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

    // Center the window
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
