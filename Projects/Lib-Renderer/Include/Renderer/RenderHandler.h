#pragma once

#ifndef SHINKIRO_RENDERER_RENDERHANDLER_H
#    define SHINKIRO_RENDERER_RENDERHANDLER_H

#    include <Renderer/_Defs.h>

#    include <Windows.h>
#    include <unordered_map>

namespace Shinkiro::Renderer
{
    class RENDER_API RenderHandler
    {
    public:
        static RenderHandler & Get();

    private:
        RenderHandler()                                    = default;
        ~RenderHandler()                                   = default;
        RenderHandler( const RenderHandler & )             = delete;
        RenderHandler & operator=( const RenderHandler & ) = delete;
    };
}

#endif
