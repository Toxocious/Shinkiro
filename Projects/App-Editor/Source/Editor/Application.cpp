#include <Editor/Application.h>

#include <Platform/InputHandler.h>

#include <Platform/Modules/Window.h>
#include <Platform/Modules/_Module.h>

#include <chrono>

#include <Platform/GL.h>

Shinkiro::Application * App = nullptr;

namespace Shinkiro
{
    Application::Application()
        : m_BundleManager( "assets.bundle" ), m_AudioManager(), m_MapManager()
    {
        Core::App = this;
    }

    Application::~Application()
    {
        Core::App = nullptr;
    }

    /**
     * Manager Accessors
     */
    Asset::AssetBundleManager & Application::GetBundleManager()
    {
        return m_BundleManager;
    }

    Audio::AudioManager & Application::GetAudioManager()
    {
        return m_AudioManager;
    }

    Renderer::MapManager & Application::GetMapManager()
    {
        return m_MapManager;
    }

    Renderer::Camera & Application::GetCamera()
    {
        return m_Camera;
    }

    Platform::InputHandler * Application::GetInputHandler()
    {
        return m_InputHandler;
    }

    Platform::Window * Application::GetWindow()
    {
        return m_Window;
    }

    /**
     * Initialize a new application.
     */
    bool Application::Initialize( const std::string & name, const std::string & version, int height, int width )
    {
        // Create instances of required modules.
        m_Window = new Shinkiro::Platform::Window();

        // Assign order of execution to modules.
        m_Modules.push_back( m_Window );

        // Initialize all of our modules.
        for ( auto module = m_Modules.begin(); module != m_Modules.end(); ++module )
        {
            bool moduleInitialized = false;

            auto p_Module = ( *module );
            SHNK_CORE_TRACE( "Initializing module '{0}'", p_Module->m_Name.c_str() );

            if ( p_Module->m_Name == "Window" )
            {
                std::string windowTitle = name;

#if defined( SHINKIRO_DEBUG )
                std::string buildType = "[DEBUG]";
#elif defined( SHINKIRO_RELEASE )
                std::string buildType = " [RELEASE]";
#elif defined( SHINKIRO_DIST )
                std::string buildType = " [DIST]";
#else
                std::string buildType = " [UNKNOWN]";
#endif

                moduleInitialized = p_Module->Initialize( windowTitle.c_str(), version, buildType, height, width );
            }
            else
            {
                moduleInitialized = p_Module->Initialize();
            }

            if ( !moduleInitialized )
            {
                SHNK_CORE_ERROR( "Failed to initialize module: {}", p_Module->m_Name );
                return false;
            }
            else
            {
                SHNK_CORE_INFO( "Module '{0}' initialized successfully", p_Module->m_Name.c_str() );
            }
        }

        m_Window->SetRenderCallback( [this]()
                                     {
                                         auto & camera = GetCamera();

                                         Shinkiro::Platform::OpenGL::glClearColor( 0.169f, 0.169f, 0.169f, 1.0f );
                                         Shinkiro::Platform::OpenGL::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                                         m_MapManager.Render( camera, m_Window->GetWidth(), m_Window->GetHeight() );

                                         //  m_Window->BeginImGuiFrame();
                                         // m_Window->RenderImGui();
                                         //  m_Window->EndImGuiFrame();
                                     } );

        m_Window->SetInputCallback( [this]( float deltaTime )
                                    {
                                        auto & camera = GetCamera();

                                        // Keyboard movement
                                        float currentSpeed = camera.GetMovementSpeed();
                                        if ( Shinkiro::Platform::InputHandler::Get().IsKeyPressed( GLFW_KEY_LEFT_SHIFT ) )
                                        {
                                            currentSpeed *= 3.0;
                                        }

                                        if ( Shinkiro::Platform::InputHandler::Get().IsKeyPressed( 'W' ) )
                                        {
                                            SHNK_CORE_TRACE( "Processing keyboard input: W" );
                                            camera.ProcessKeyboard( Shinkiro::Renderer::FORWARD, currentSpeed * deltaTime );
                                        }
                                        if ( Shinkiro::Platform::InputHandler::Get().IsKeyPressed( 'S' ) )
                                        {
                                            SHNK_CORE_TRACE( "Processing keyboard input: W" );
                                            camera.ProcessKeyboard( Shinkiro::Renderer::BACKWARD, currentSpeed * deltaTime );
                                        }
                                        if ( Shinkiro::Platform::InputHandler::Get().IsKeyPressed( 'A' ) )
                                        {
                                            SHNK_CORE_TRACE( "Processing keyboard input: W" );
                                            camera.ProcessKeyboard( Shinkiro::Renderer::LEFT, currentSpeed * deltaTime );
                                        }
                                        if ( Shinkiro::Platform::InputHandler::Get().IsKeyPressed( 'D' ) )
                                        {
                                            SHNK_CORE_TRACE( "Processing keyboard input: W" );
                                            camera.ProcessKeyboard( Shinkiro::Renderer::RIGHT, currentSpeed * deltaTime );
                                        }

                                        // Mouse rotation (only when right mouse button is held)
                                        if ( Shinkiro::Platform::InputHandler::Get().IsMouseButtonPressed( GLFW_MOUSE_BUTTON_RIGHT ) )
                                        {
                                            glfwSetInputMode( m_Window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );

                                            if ( m_Window->IsFirstMouse() )
                                            {
                                                m_Window->SetLastMousePos( m_Window->GetMousePos() );
                                                m_Window->SetFirstMouse( false );
                                            }

                                            glm::vec2 mousePos     = m_Window->GetMousePos();
                                            glm::vec2 lastMousePos = m_Window->GetLastMousePos();

                                            float xoffset = mousePos.x - lastMousePos.x;
                                            float yoffset = lastMousePos.y - mousePos.y;

                                            m_Window->SetLastMousePos( mousePos );

                                            SHNK_CORE_TRACE( "Processing mouse movement." );
                                            camera.ProcessMouseMovement( xoffset, yoffset );
                                        }
                                        else
                                        {
                                            glfwSetInputMode( m_Window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
                                            m_Window->SetFirstMouse( true );
                                        }
                                    } );

        // Initialize the Map Manager.
        m_MapManager.Initialize();

        return true;
    }

    /**
     * Start the application after initialization.
     */
    bool Application::Start()
    {
        for ( auto module = m_Modules.begin(); module != m_Modules.end(); ++module )
        {
            auto p_Module = ( *module );
            SHNK_CORE_TRACE( "Starting module '{0}'", p_Module->m_Name.c_str() );
            p_Module->Start();
        }

        if ( GetMapManager().LoadMap( "Maps/DecorTest.tmx" ) )
        {
            GetMapManager().SetActiveMap( "Maps/DecorTest.tmx" );
        }

        {
            const auto AtmosphereBGM = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Audio/BGM/Atmosphere-Crystal.wav" );
            Shinkiro::Core::App->GetAudioManager().PlayOST( AtmosphereBGM );
            Shinkiro::Core::App->GetAudioManager().SetOSTVolume( 0.1f );

            const auto AbsolCry = Shinkiro::Core::App->GetBundleManager().GetAssetData( "Audio/Cries/absol.wav" );
            Shinkiro::Core::App->GetAudioManager().PlaySoundAsync( AbsolCry );
        }

        return true;
    }

    /**
     * Clean up and shutdown the application.
     */
    bool Application::CleanUp()
    {
        SHNK_CORE_TRACE( "Cleaning up the application and its modules" );
        {
            m_MapManager.Shutdown();

            for ( auto module = m_Modules.begin(); module != m_Modules.end(); ++module )
            {
                auto p_Module = ( *module );
                SHNK_CORE_TRACE( "Cleaning module '{0}'", p_Module->m_Name.c_str() );

                p_Module->ShutdownImGui();
                p_Module->CleanUp();
            }

            m_Modules.clear();

            delete m_Window;
            m_Window = nullptr;
        }

        return true;
    }

    /**
     * Update loop.
     */
    Shinkiro::Core::UpdateStatus Application::Update()
    {
        Shinkiro::Core::UpdateStatus status = Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;

        // Pre-Update
        {
            for ( auto module = m_Modules.begin(); module != m_Modules.end(); ++module )
            {
                auto p_Module = ( *module );

                status = p_Module->PreUpdate();
                if ( status != Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE )
                {
                    SHNK_CORE_ERROR( "Module '{0}' failed on PreUpdate() -> '{1}'", p_Module->m_Name, p_Module->UpdateStatusToString( status ).c_str() );
                    return status;
                }
            }
        }

        // Update
        {
            for ( auto module = m_Modules.begin(); module != m_Modules.end(); ++module )
            {
                auto p_Module = ( *module );

                status = p_Module->Update();
                if ( status != Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE )
                {
                    SHNK_CORE_ERROR( "Module '{0}' failed on Update() -> '{1}'", p_Module->m_Name, p_Module->UpdateStatusToString( status ).c_str() );
                    return status;
                }
            }
        }

        // Post-Update
        {
            for ( auto module = m_Modules.begin(); module != m_Modules.end(); ++module )
            {
                auto p_Module = ( *module );

                status = p_Module->PostUpdate();
                if ( status != Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE )
                {
                    SHNK_CORE_ERROR( "Module '{0}' failed on PostUpdate() -> '{1}'", p_Module->m_Name, p_Module->UpdateStatusToString( status ).c_str() );
                    return status;
                }
            }
        }

        return status;
    }
}
