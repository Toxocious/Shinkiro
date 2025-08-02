#include <Core/Application.h>

Shinkiro::Core::Application * App = nullptr;

namespace Shinkiro::Core
{
    Application::Application()
        : m_BundleManager( "assets.bundle" )
    {
        App = this;

        m_BundleManager.LoadAssetsIntoMemory();
    }

    Application::~Application()
    {
        App = nullptr;
    }

    /**
     * Initialize a new application.
     */
    bool Application::Initialize( std::string name, std::string version, int height, int width )
    {
        // Create instances of required modules.
        m_Window = new Shinkiro::Core::Window();

        // Assign order of execution to modules.
        m_Modules.push_back( m_Window );

        // Initialize all of our modules.
        for ( auto module = m_Modules.begin(); module != m_Modules.end(); ++module )
        {
            auto p_Module = ( *module );
            SHNK_CORE_TRACE( "Initializing module '{0}'", p_Module->m_Name.c_str() );

            if ( p_Module->m_Name == "Window" )
            {
                name += " " + version;

#if defined( SHINKIRO_DEBUG )
                name += " [DEBUG]";
#elif defined( SHINKIRO_RELEASE )
                name += " [RELEASE]";
#elif defined( SHINKIRO_DIST )
                name += " [DIST]";
#else
                name += " [UNKNOWN]";
#endif

                p_Module->Initialize( name.c_str(), height, width );
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
            const auto AtmosphereBGM = App->m_BundleManager.GetAssetData( "Audio/BGM/Atmosphere-Crystal.wav" );
            // m_AudioManager.PlaySoundAsync( AtmosphereBGM );
            m_AudioManager.PlayOST( AtmosphereBGM );

            const auto AbsolCry = App->m_BundleManager.GetAssetData( "Audio/Cries/absol.wav" );
            m_AudioManager.PlaySoundAsync( AbsolCry );
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
