#include <Renderer/RenderHandler.h>

namespace Shinkiro::Renderer
{
    RenderHandler & RenderHandler::Get()
    {
        static RenderHandler instance;
        return instance;
    }
}
