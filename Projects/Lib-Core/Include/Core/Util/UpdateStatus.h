#pragma once

#ifndef SHINKIRO_CORE_UPDATESTATUS_H
#    define SHINKIRO_CORE_UPDATESTATUS_H

#    include <Core/_Defs.h>

extern "C"
{
    namespace Shinkiro::Core
    {
        enum class CORE_API UpdateStatus
        {
            UPDATE_CONTINUE,
            UPDATE_STOP,
            UPDATE_ERROR
        };
    }
}

#endif
