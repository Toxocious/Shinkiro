#include <Renderer/_Common.h>

#include <Renderer/Camera/Camera.h>
#include <Renderer/Camera/CameraFactory.h>

namespace Shinkiro::Renderer
{
    std::unique_ptr<ICamera> CreateCamera()
    {
        return std::make_unique<Camera>();
    }
}
