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

        /**
         * @brief Constructs a plane from the given coefficients and normalizes it
         * @param a Coefficient A
         * @param b Coefficient B
         * @param c Coefficient C
         * @param d Coefficient D
         */
        Plane( float a, float b, float c, float d )
        {
            glm::vec3 n( a, b, c );
            float     mag = glm::length( n );
            normal        = n / mag;
            distance      = d / mag;
        }

        /**
         * @brief Calculates the signed distance from a point to the plane
         * @param point The point in 3D space
         * @return The signed distance from the point to the plane
         */
        float GetSignedDistanceToPlane( const glm::vec3 & point ) const
        {
            return glm::dot( normal, point ) + distance;
        }
    };

    class Frustum
    {
    public:
        /**
         * @brief Updates the frustum planes
         * @param viewProjectionMatrix The combined view and projection matrix
         */
        void Update( const glm::mat4 & viewProjectionMatrix );

        /**
         * @brief Checks if a sphere is inside the frustum
         * @param center The center of the sphere
         * @param radius The radius of the sphere
         * @return True if the sphere is inside or intersects the frustum, false otherwise
         */
        bool IsSphereInFrustum( const glm::vec3 & center, float radius ) const;

    public:
        Plane m_TopFace;
        Plane m_BottomFace;
        Plane m_RightFace;
        Plane m_LeftFace;
        Plane m_FarFace;
        Plane m_NearFace;
    };
}

#endif
