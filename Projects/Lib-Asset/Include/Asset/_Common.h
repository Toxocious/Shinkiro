#pragma once
#ifndef SHINKIRO_ASSET_PCH
#    define SHINKIRO_ASSET_PCH

#    include <Asset/_Defs.h>

#ifndef SHINKIRO_STB_IMPL
#    define SHINKIRO_STB_IMPL

#    include <stb_image/stb_image.h>
#endif

#    include <zstd/zstd.h>

#    ifndef NOMINMAX
#        define NOMINMAX
#    endif
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif

#    include <Windows.h>

#    include <algorithm>
#    include <chrono>
#    include <filesystem>
#    include <fstream>
#    include <iostream>
#    include <map>
#    include <mutex>
#    include <sstream>
#    include <stack>
#    include <stdexcept>
#    include <string>
#    include <unordered_map>
#    include <vector>

#endif
