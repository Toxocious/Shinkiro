#pragma once
#ifndef SHINKIRO_PLATFORM_PCH
#    define SHINKIRO_PLATFORM_PCH

#    include <Platform/_Defs.h>

#    include <Core/Util/UpdateStatus.h>

#    ifndef SHINKIRO_IMPL_GLAD
#        define SHINKIRO_IMPL_GLAD
#        include <glad/glad.h>
#    endif

#    ifndef SHINKIRO_IMPL_GLFW
#        define SHINKIRO_IMPL_GLFW
#        include <glfw/glfw3.h>

#        include <gl/GL.h>
#    endif

#    include <glm/glm.hpp>
#    include <stb_image/stb_image.h>
#    include <stb_image/stb_image_resize.h>

#    include <Windows.h>

#    include <chrono>
#    include <iostream>
#    include <memory>
#    include <string>
#    include <unordered_map>

#endif
