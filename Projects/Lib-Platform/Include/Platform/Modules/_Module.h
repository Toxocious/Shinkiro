#pragma once
#ifndef SHINKIRO_PLATFORM_MODULE_H
#    define SHINKIRO_PLATFORM_MODULE_H

#    include <Platform/_Defs.h>

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

        virtual Shinkiro::Platform::UpdateStatus PreUpdate();
        virtual Shinkiro::Platform::UpdateStatus Update();
        virtual Shinkiro::Platform::UpdateStatus PostUpdate();

        static std::string UpdateStatusToString( Shinkiro::Platform::UpdateStatus us );

    public:
        std::string m_Name;

    private:
        bool m_Enabled = true;
    };
}

#endif
