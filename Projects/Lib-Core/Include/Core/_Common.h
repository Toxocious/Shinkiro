#pragma once
#ifndef SHINKIRO_CORE_PCH
#    define SHINKIRO_CORE_PCH

#    include <Core/_Defs.h>

#    include <filesystem>
#    include <iostream>
#    include <memory>

#    ifndef SHINKIRO_IMPL_GLAD
#        define SHINKIRO_IMPL_GLAD
#        include <glad/glad.h>
#    endif

#    ifndef SHINKIRO_IMPL_GLFW
#        define SHINKIRO_IMPL_GLFW
#        include <glfw/glfw3.h>

#        include <gl/GL.h>
#    endif

#    ifndef SHINKIRO_LOGGER_HEADERS
#        define SHINKIRO_LOGGER_HEADERS
#        include <Log/Log.h>
#        include <Log/Util/Assert.h>
#        include <Log/Util/Macro.h>
#    endif

#endif
