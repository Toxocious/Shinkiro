#include <Platform/Modules/_Module.h>

#include <string>

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

    bool Module::Initialize( const char * title, int height, int width )
    {
        return true;
    }

    bool Module::Start()
    {
        return true;
    }

    Shinkiro::Platform::UpdateStatus Module::PreUpdate()
    {
        return Shinkiro::Platform::UpdateStatus::UPDATE_CONTINUE;
    }

    Shinkiro::Platform::UpdateStatus Module::Update()
    {
        return Shinkiro::Platform::UpdateStatus::UPDATE_CONTINUE;
    }

    Shinkiro::Platform::UpdateStatus Module::PostUpdate()
    {
        return Shinkiro::Platform::UpdateStatus::UPDATE_CONTINUE;
    }

    bool Module::CleanUp()
    {
        return true;
    }

    std::string Module::UpdateStatusToString( Shinkiro::Platform::UpdateStatus updateStatus )
    {
        switch ( updateStatus )
        {
            case Shinkiro::Platform::UpdateStatus::UPDATE_CONTINUE:
                return std::string( "Update Continue" );
            case Shinkiro::Platform::UpdateStatus::UPDATE_STOP:
                return std::string( "Update Stop" );
            case Shinkiro::Platform::UpdateStatus::UPDATE_ERROR:
                return std::string( "Update Error" );
        }

        return std::string( "NONE" );
    }
}
