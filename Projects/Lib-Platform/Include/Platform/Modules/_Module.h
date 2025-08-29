#pragma once
#ifndef SHINKIRO_PLATFORM_MODULE_H
#    define SHINKIRO_PLATFORM_MODULE_H

#    include <Platform/_Defs.h>

#    include <Core/Util/UpdateStatus.h>

#    include <string>

namespace Shinkiro::Platform
{
    class PLATFORM_API Module
    {
    public:
        Module( const char * name, bool enabled );
        virtual ~Module();

        virtual bool Initialize();
        virtual bool Initialize( const char * title, int height, int width );

        virtual bool Start();
        virtual bool CleanUp();

        virtual Shinkiro::Core::UpdateStatus PreUpdate()  = 0;
        virtual Shinkiro::Core::UpdateStatus Update()     = 0;
        virtual Shinkiro::Core::UpdateStatus PostUpdate() = 0;

        static std::string UpdateStatusToString( Shinkiro::Core::UpdateStatus us );

    public:
        std::string m_Name;

    private:
        bool m_Enabled = true;
    };
}

#endif
