#pragma once

#ifndef SHINKIRO_RENDERER_ICAMERA_H
#    define SHINKIRO_RENDERER_ICAMERA_H

#    include <Core/_Defs.h>

#    include <glm/glm.hpp>
#    include <glm/gtc/matrix_transform.hpp>

namespace Shinkiro::Renderer
{
    class Frustum;

    enum Camera_Movement;

    class CORE_API ICamera
    {
    public:
        ICamera()          = default;
        virtual ~ICamera() = default;

        // Setters
        virtual const glm::mat4 SetProjection( const glm::mat4 & projection ) = 0;
        virtual const glm::vec3 SetPosition( const glm::vec3 & position )     = 0;
        virtual const glm::vec3 SetUp( const glm::vec3 & up )                 = 0;
        virtual const float     SetPitch( float pitch )                       = 0;
        virtual const float     SetYaw( float yaw )                           = 0;
        virtual const float     SetZoom( float zoom )                         = 0;

        // Getters
        virtual glm::mat4         GetViewMatrix() const    = 0;
        virtual const glm::vec3 & GetPosition() const      = 0;
        virtual const Frustum &   GetFrustum() const       = 0;
        virtual float             GetPitch() const         = 0;
        virtual float             GetYaw() const           = 0;
        virtual float             GetZoom() const          = 0;
        virtual const float       GetMovementSpeed() const = 0;

        // Input / updates
        // direction parameter type should match your concrete implementation (e.g., enum or int)
        virtual void ProcessKeyboard( Camera_Movement direction, float amount )                = 0;
        virtual void ProcessMouseMovement( float xoffset, float yoffset, bool constrainPitch ) = 0;
        virtual void updateCameraVectors()                                                     = 0;
    };
}

#endif
