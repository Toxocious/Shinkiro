#include <Core/Application.h>

namespace Shinkiro::Core
{
    Application::Application( std::string name, std::string version, int height, int width )
        : m_Name( name ), m_Version( version ), m_Height( height ), m_Width( width )
    {
    }

    Application::~Application()
    {
    }

    /**
     * Initialize a new application.
     */
    bool Application::Initialize()
    {
        return true;
    }

    /**
     * Start the application after initialization.
     */
    bool Application::Start()
    {
        return true;
    }

    /**
     * Clean up and shutdown the application.
     */
    bool Application::CleanUp()
    {
        return true;
    }

    /**
     * Pre-update loop.
     */
    bool Application::PreUpdate()
    {
        return true;
    }

    /**
     * Update loop.
     */
    bool Application::Update()
    {
        return true;
    }

    /**
     * Post-update loop.
     */
    bool Application::PostUpdate()
    {
        return true;
    }
}
