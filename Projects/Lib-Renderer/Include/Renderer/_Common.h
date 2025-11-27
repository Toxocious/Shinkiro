#pragma once
#ifndef SHINKIRO_RENDERER_PCH
#    define SHINKIRO_RENDERER_PCH

#    include <Renderer/_Defs.h>

#    include <Core/Util/UpdateStatus.h>

#    ifndef SHINKIRO_IMPL_GLAD
#        define SHINKIRO_IMPL_GLAD
#        include <glad/glad.h>
#    endif

#    ifndef SHINKIRO_IMPL_GLFW
#        define SHINKIRO_IMPL_GLFW
#        include <glfw/glfw3.h>

#        include <GL/gl.h>
#    endif

#    include <glm/glm.hpp>
#    include <glm/gtc/matrix_transform.hpp>
#    include <glm/gtc/type_ptr.hpp>

#    include <stb_image/stb_image.h>

#    include <Windows.h>

#    include <algorithm>
#    include <filesystem>
#    include <functional>
#    include <set>
#    include <sstream>
#    include <string>
#    include <vector>

#endif

