#include <Platform/_Common.h>

#include <Platform/Modules/Window.h>

#include <Core/IApplication.h>

#include <Asset/AssetBundleManager.h>

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

    bool Window::Initialize( const char * title, int height, int width )
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

        m_Window.reset( glfwCreateWindow( m_Width, m_Height, m_Title, nullptr, nullptr ) );
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
            std::cerr << "Failed to initialize GLAD" << std::endl;
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

        SetWindowIcon();
        CenterWindow();

        InitializeImGui();

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
                    RenderImGui();

                    std::this_thread::sleep_for( std::chrono::seconds( 3 ) );

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
        glClearColor( 0.169f, 0.169f, 0.169f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void Window::RenderImGui()
    {
        BeginImGuiFrame();

        {
            glClearColor( 0.169f, 0.169f, 0.169f, 1.0f );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );
            ImGui::SetNextWindowSize( ImGui::GetIO().DisplaySize );
            ImGui::Begin( "LoadingScreen", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground );

            float windowWidth  = ImGui::GetWindowSize().x;
            float windowHeight = ImGui::GetWindowSize().y;

            // Calculate total height of all elements for vertical centering
            float titleTextHeight   = ImGui::CalcTextSize( m_Title ).y;
            float loadingTextHeight = ImGui::CalcTextSize( "Loading..." ).y;
            // float totalContentHeight = m_iconHeight + titleTextHeight + loadingTextHeight + ImGui::GetStyle().ItemSpacing.y * 2;

            // ImGui::SetCursorPosY( ( windowHeight - totalContentHeight ) * 0.5f );

            // // 1. Icon
            // if ( m_iconTextureID != 0 )
            // {
            //     ImGui::SetCursorPosX( ( windowWidth - m_iconWidth ) * 0.5f );
            //     ImGui::Image( ( intptr_t ) m_iconTextureID, ImVec2( ( float ) m_iconWidth, ( float ) m_iconHeight ) );
            // }

            // 2. Application Title
            float titleTextWidth = ImGui::CalcTextSize( m_Title ).x;
            ImGui::SetCursorPosX( ( windowWidth - titleTextWidth ) * 0.5f );
            ImGui::Text( "%s", m_Title );

            // 3. "Loading..." text
            float loadingTextWidth = ImGui::CalcTextSize( "Loading..." ).x;
            ImGui::SetCursorPosX( ( windowWidth - loadingTextWidth ) * 0.5f );
            ImGui::Text( "Loading..." );

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
            glfwSwapBuffers( GetGLFWWindow() );

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

        EndImGuiFrame();
    }

    void Window::InitializeImGui()
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
    }

    void Window::ShutdownImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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
            glfwSetWindowTitle( m_Window.get(), m_Title );
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

    void Window::SetWindowIcon()
    {
        const auto ShinkiroWindowIcon = Shinkiro::Core::App->GetBundleManager().GetAssetData( "ShinkiroWindowIcon.png" );

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
            GLFWimage images[1];
            images[0].width  = iconWidth;
            images[0].height = iconHeight;
            images[0].pixels = m_WindowIcon;

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
