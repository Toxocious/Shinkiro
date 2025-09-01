#include <Platform/_Common.h>

#include <Platform/Modules/_Module.h>

namespace Shinkiro::Platform
{
    Module::Module( const char * name, bool enabled )
        : m_Name( name ), m_Enabled( enabled )
    {
    }

    Module::~Module()
    {
    }

    bool Module::Initialize()
    {
        return true;
    }

    bool Module::Initialize( const std::string & title, const std::string & version, const std::string build_type, int height, int width )
    {
        return true;
    }

    bool Module::Start()
    {
        return true;
    }

    bool Module::CleanUp()
    {
        return true;
    }

    bool Module::InitializeImGui()
    {
        return true;
    }

    bool Module::ShutdownImGui()
    {
        return true;
    }

    Shinkiro::Core::UpdateStatus Module::PreUpdate()
    {
        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    Shinkiro::Core::UpdateStatus Module::Update()
    {
        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    Shinkiro::Core::UpdateStatus Module::PostUpdate()
    {
        return Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE;
    }

    std::string Module::UpdateStatusToString( Shinkiro::Core::UpdateStatus updateStatus )
    {
        switch ( updateStatus )
        {
            case Shinkiro::Core::UpdateStatus::UPDATE_CONTINUE:
                return std::string( "Update Continue" );
            case Shinkiro::Core::UpdateStatus::UPDATE_STOP:
                return std::string( "Update Stop" );
            case Shinkiro::Core::UpdateStatus::UPDATE_ERROR:
                return std::string( "Update Error" );
        }

        return std::string( "NONE" );
    }
}
