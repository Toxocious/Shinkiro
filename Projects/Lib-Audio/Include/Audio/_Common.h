#pragma once
#ifndef SHINKIRO_AUDIO_PCH
#    define SHINKIRO_AUDIO_PCH

#    include <Audio/_Defs.h>

#    include <miniaudio/miniaudio.h>

#    ifndef NOMINMAX
#        define NOMINMAX
#    endif

#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif

#    include <Windows.h>

#    include <atomic>
#    include <condition_variable>
#    include <cstdint>
#    include <filesystem>
#    include <fstream>
#    include <iostream>
#    include <mutex>
#    include <queue>
#    include <stack>
#    include <thread>
#    include <vector>

#endif
