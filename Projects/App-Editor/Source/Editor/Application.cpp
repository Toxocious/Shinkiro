#include <Editor/Application.h>

#include <Platform/Modules/Window.h>
#include <Platform/Modules/_Module.h>

#include <chrono>

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
            auto p_Module = ( *module );
            SHNK_CORE_TRACE( "Initializing module '{0}'", p_Module->m_Name.c_str() );

            if ( p_Module->m_Name == "Window" )
            {
                std::string windowTitle = name + " v" + version;

#if defined( SHINKIRO_DEBUG )
                windowTitle += " [DEBUG]";
#elif defined( SHINKIRO_RELEASE )
                windowTitle += " [RELEASE]";
#elif defined( SHINKIRO_DIST )
                windowTitle += " [DIST]";
#else
                windowTitle += " [UNKNOWN]";
#endif

                p_Module->Initialize( windowTitle.c_str(), height, width );
            }
            else
            {
                p_Module->Initialize();
            }
        }

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
            for ( auto module = m_Modules.begin(); module != m_Modules.end(); ++module )
            {
                auto p_Module = ( *module );
                SHNK_CORE_TRACE( "Cleaning module '{0}'", p_Module->m_Name.c_str() );
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
