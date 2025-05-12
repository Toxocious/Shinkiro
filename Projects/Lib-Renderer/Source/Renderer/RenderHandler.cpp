#include <Renderer/RenderHandler.h>

RenderHandler& RenderHandler::Get() {
    static RenderHandler instance;
    return instance;
}
