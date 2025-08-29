#pragma once

#ifndef SHINKIRO_RENDERER_CAMERA_FRUSTUM_H
#    define SHINKIRO_RENDERER_CAMERA_FRUSTUM_H

#    include <glm/glm.hpp>

namespace Shinkiro::Renderer
{
    // Represents a single plane in 3D space (Ax + By + Cz + D = 0)
    struct Plane
    {
        glm::vec3 normal   = { 0.f, 1.f, 0.f };
        float     distance = 0.f;

        Plane() = default;

        Plane( float a, float b, float c, float d )
        {
            glm::vec3 n( a, b, c );
            float     mag = glm::length( n );
            normal        = n / mag;
            distance      = d / mag;
        }

        float getSignedDistanceToPlane( const glm::vec3 & point ) const
        {
            return glm::dot( normal, point ) + distance;
        }
    };

    class Frustum
    {
    public:
        Plane topFace;
        Plane bottomFace;
        Plane rightFace;
        Plane leftFace;
        Plane farFace;
        Plane nearFace;

        void Update( const glm::mat4 & viewProjectionMatrix );
        bool IsSphereInFrustum( const glm::vec3 & center, float radius ) const;
    };
}

#endif
