#include <Renderer/_Common.h>

#include <Renderer/Camera/Frustum.h>

namespace Shinkiro::Renderer
{
    void Frustum::Update( const glm::mat4 & viewProjectionMatrix )
    {
        const glm::mat4 & m = viewProjectionMatrix;

        // Left Frustum Plane
        m_LeftFace = Plane(
            m[0][3] + m[0][0],
            m[1][3] + m[1][0],
            m[2][3] + m[2][0],
            m[3][3] + m[3][0]
        );

        // Right Frustum Plane
        m_RightFace = Plane(
            m[0][3] - m[0][0],
            m[1][3] - m[1][0],
            m[2][3] - m[2][0],
            m[3][3] - m[3][0]
        );

        // Bottom Frustum Plane
        m_BottomFace = Plane(
            m[0][3] + m[0][1],
            m[1][3] + m[1][1],
            m[2][3] + m[2][1],
            m[3][3] + m[3][1]
        );

        // Top Frustum Plane
        m_TopFace = Plane(
            m[0][3] - m[0][1],
            m[1][3] - m[1][1],
            m[2][3] - m[2][1],
            m[3][3] - m[3][1]
        );

        // Near Frustum Plane
        m_NearFace = Plane(
            m[0][3] + m[0][2],
            m[1][3] + m[1][2],
            m[2][3] + m[2][2],
            m[3][3] + m[3][2]
        );

        // Far Frustum Plane
        m_FarFace = Plane(
            m[0][3] - m[0][2],
            m[1][3] - m[1][2],
            m[2][3] - m[2][2],
            m[3][3] - m[3][2]
        );
    }

    bool Frustum::IsSphereInFrustum( const glm::vec3 & center, float radius ) const
    {
        if ( m_TopFace.GetSignedDistanceToPlane( center ) < -radius || m_BottomFace.GetSignedDistanceToPlane( center ) < -radius || m_RightFace.GetSignedDistanceToPlane( center ) < -radius || m_LeftFace.GetSignedDistanceToPlane( center ) < -radius || m_FarFace.GetSignedDistanceToPlane( center ) < -radius || m_NearFace.GetSignedDistanceToPlane( center ) < -radius )
        {
            return false;
        }

        return true;
    }
}
