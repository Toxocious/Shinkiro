#pragma once

#ifndef SHINKIRO_RENDERER_CAMERAFACTORY_H
#    define SHINKIRO_RENDERER_CAMERAFACTORY_H

#    include <Renderer/_Defs.h>

#    include <Core/Interfaces/CameraInterface.h>

#    include <memory>

namespace Shinkiro::Renderer
{
    RENDER_API std::unique_ptr<ICamera> CreateCamera();
}

#endif
