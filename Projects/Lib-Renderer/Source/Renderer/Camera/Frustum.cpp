#include <Renderer/_Common.h>

#include <Renderer/Camera/Frustum.h>

namespace Shinkiro::Renderer
{
    void Frustum::Update( const glm::mat4 & viewProjectionMatrix )
    {
        const glm::mat4 & m = viewProjectionMatrix;

        // Left Frustum Plane
        leftFace = Plane(
            m[0][3] + m[0][0],
            m[1][3] + m[1][0],
            m[2][3] + m[2][0],
            m[3][3] + m[3][0]
        );

        // Right Frustum Plane
        rightFace = Plane(
            m[0][3] - m[0][0],
            m[1][3] - m[1][0],
            m[2][3] - m[2][0],
            m[3][3] - m[3][0]
        );

        // Bottom Frustum Plane
        bottomFace = Plane(
            m[0][3] + m[0][1],
            m[1][3] + m[1][1],
            m[2][3] + m[2][1],
            m[3][3] + m[3][1]
        );

        // Top Frustum Plane
        topFace = Plane(
            m[0][3] - m[0][1],
            m[1][3] - m[1][1],
            m[2][3] - m[2][1],
            m[3][3] - m[3][1]
        );

        // Near Frustum Plane
        nearFace = Plane(
            m[0][3] + m[0][2],
            m[1][3] + m[1][2],
            m[2][3] + m[2][2],
            m[3][3] + m[3][2]
        );

        // Far Frustum Plane
        farFace = Plane(
            m[0][3] - m[0][2],
            m[1][3] - m[1][2],
            m[2][3] - m[2][2],
            m[3][3] - m[3][2]
        );
    }

    bool Frustum::IsSphereInFrustum( const glm::vec3 & center, float radius ) const
    {
        // Check if the sphere is outside any of the frustum planes.
        if ( topFace.getSignedDistanceToPlane( center ) < -radius )
        {
            return false;
        }
        if ( bottomFace.getSignedDistanceToPlane( center ) < -radius )
        {
            return false;
        }
        if ( rightFace.getSignedDistanceToPlane( center ) < -radius )
        {
            return false;
        }
        if ( leftFace.getSignedDistanceToPlane( center ) < -radius )
        {
            return false;
        }
        if ( farFace.getSignedDistanceToPlane( center ) < -radius )
        {
            return false;
        }
        if ( nearFace.getSignedDistanceToPlane( center ) < -radius )
        {
            return false;
        }

        return true;
    }
}
