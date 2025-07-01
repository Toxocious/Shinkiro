#pragma once
#ifndef SHINKIRO_CORE_MODULE_H
#    define SHINKIRO_CORE_MODULE_H

#    include <Core/_Defs.h>

#    include <Core/Util/UpdateStatus.h>

#    include <string>

namespace Shinkiro::Core
{
    class CORE_API Module
    {
    public:
        Module( const char * name, bool enabled );
        virtual ~Module();

        virtual bool Initialize();
        virtual bool Initialize( const char * title, int height, int width );

        virtual bool Start();
        virtual bool CleanUp();

        virtual Shinkiro::Core::UpdateStatus PreUpdate();
        virtual Shinkiro::Core::UpdateStatus Update();
        virtual Shinkiro::Core::UpdateStatus PostUpdate();

        static std::string UpdateStatusToString( Shinkiro::Core::UpdateStatus us );

    public:
        std::string m_Name;

    private:
        bool m_Enabled = true;
    };
}

#endif
