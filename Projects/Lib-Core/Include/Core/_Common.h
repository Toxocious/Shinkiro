#pragma once
#ifndef SHINKIRO_CORE_PCH
#    define SHINKIRO_CORE_PCH

#    include <Core/_Defs.h>

#    include <filesystem>
#    include <iostream>
#    include <memory>

#    ifndef SHINKIRO_LOGGER_HEADERS
#        define SHINKIRO_LOGGER_HEADERS
#        include <Core/Util/Log.h>

#        include <Core/Util/Assert.h>
#        include <Core/Util/Macro.h>
#    endif

#endif
