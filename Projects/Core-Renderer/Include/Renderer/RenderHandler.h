#pragma once

#include <Windows.h>
#include <unordered_map>

#ifdef PLATFORM_WINDOWS
    #ifdef CORE_RENDER_EXPORTS
        #define RENDER_API __declspec(dllexport)
    #else
        #define RENDER_API __declspec(dllimport)
    #endif
#else
    #define RENDER_API
#endif

class RENDER_API RenderHandler {
public:
    static RenderHandler& Get();

private:
    RenderHandler() = default;
    ~RenderHandler() = default;
    RenderHandler(const RenderHandler&) = delete;
    RenderHandler& operator=(const RenderHandler&) = delete;
};
