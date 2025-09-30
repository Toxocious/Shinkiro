#include <Renderer/_Common.h>

#include <Renderer/Camera/Camera.h>

#include <Core/Util/Log.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>

namespace Shinkiro::Renderer
{
    Camera::Camera( glm::vec3 position, glm::vec3 up, float yaw, float pitch )
        : m_Front( glm::vec3( 0.0f, 0.0f, -1.0f ) ), m_MovementSpeed( SPEED ), m_MouseSensitivity( SENSITIVITY ), m_Zoom( ZOOM )
    {
        m_Position = position;
        m_WorldUp  = up;
        m_Yaw      = yaw;
        m_Pitch    = pitch;

        updateCameraVectors();
    }

    void Camera::ProcessKeyboard( Camera_Movement direction, float deltaTime )
    {
        float velocity = m_MovementSpeed * deltaTime;
        if ( direction == FORWARD )
        {
            m_Position += m_Front * velocity;
        }
        if ( direction == BACKWARD )
        {
            m_Position -= m_Front * velocity;
        }
        if ( direction == LEFT )
        {
            m_Position -= m_Right * velocity;
        }
        if ( direction == RIGHT )
        {
            m_Position += m_Right * velocity;
        }
    }

    void Camera::ProcessMouseMovement( float xoffset, float yoffset, GLboolean constrainPitch )
    {
        SHNK_CORE_INFO( "Processing mouse movement: xoffset={}, yoffset={}", xoffset, yoffset );

        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw   += xoffset;
        m_Pitch += yoffset;

        if ( constrainPitch )
        {
            if ( m_Pitch > 89.0f )
            {
                m_Pitch = 89.0f;
            }
            if ( m_Pitch < -89.0f )
            {
                m_Pitch = -89.0f;
            }
        }

        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll( float yoffset )
    {
        m_Zoom -= ( float ) yoffset;
        if ( m_Zoom < 1.0f )
        {
            m_Zoom = 1.0f;
        }
        if ( m_Zoom > 45.0f )
        {
            m_Zoom = 45.0f;
        }
    }

    void Camera::updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos( glm::radians( m_Yaw ) ) * cos( glm::radians( m_Pitch ) );
        front.y = sin( glm::radians( m_Pitch ) );
        front.z = sin( glm::radians( m_Yaw ) ) * cos( glm::radians( m_Pitch ) );

        m_Front = glm::normalize( front );
        m_Right = glm::normalize( glm::cross( m_Front, m_WorldUp ) );
        m_Up    = glm::normalize( glm::cross( m_Right, m_Front ) );
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt( m_Position, m_Position + m_Front, m_Up );
    }

    glm::mat4 Camera::GetProjectionMatrix( int screenWidth, int screenHeight ) const
    {
        if ( screenHeight == 0 )
        {
            screenHeight = 1;
        }

        float aspectRatio = ( float ) screenWidth / ( float ) screenHeight;

        return glm::perspective( glm::radians( m_Zoom ), aspectRatio, 0.1f, 1000.0f );
    }

    std::pair<glm::vec3, glm::vec3> Camera::GetMouseRay( float mouseX, float mouseY, int screenWidth, int screenHeight ) const
    {
        float x = ( 2.0f * mouseX ) / screenWidth - 1.0f;
        float y = 1.0f - ( 2.0f * mouseY ) / screenHeight; // Y is inverted
        float z = 1.0f;

        glm::vec3 ray_nds  = glm::vec3( x, y, z );
        glm::vec4 ray_clip = glm::vec4( ray_nds.x, ray_nds.y, -1.0, 1.0 );

        glm::vec4 ray_eye = glm::inverse( GetProjectionMatrix( screenWidth, screenHeight ) ) * ray_clip;
        ray_eye           = glm::vec4( ray_eye.x, ray_eye.y, -1.0, 0.0 );

        glm::vec3 ray_wor = glm::vec3( glm::inverse( GetViewMatrix() ) * ray_eye );
        ray_wor           = glm::normalize( ray_wor );

        return { m_Position, ray_wor };
    }

    glm::vec3 Camera::GetMousePositionInWorld( float mouseX, float mouseY, int screenWidth, int screenHeight ) const
    {
        auto [rayOrigin, rayDirection] = GetMouseRay( mouseX, mouseY, screenWidth, screenHeight );

        glm::vec3 planeOrigin( 0.0f, 0.0f, 0.0f );
        glm::vec3 planeNormal( 0.0f, 1.0f, 0.0f );

        float intersectionDistance;
        bool  intersects = glm::intersectRayPlane( rayOrigin, rayDirection, planeOrigin, planeNormal, intersectionDistance );

        if ( intersects )
        {
            return rayOrigin + rayDirection * intersectionDistance;
        }

        return glm::vec3( 0.0f );
    }

    const float Camera::GetMovementSpeed() const
    {
        return m_MovementSpeed;
    }

    const Frustum & Camera::GetFrustum() const
    {
        return m_Frustum;
    }

    const glm::mat4 Camera::SetProjection( const glm::mat4 & projection )
    {
        m_Projection = projection;
        return m_Projection;
    }

    const glm::vec3 & Camera::GetPosition() const
    {
        return m_Position;
    }

    const glm::vec3 Camera::SetPosition( const glm::vec3 & position )
    {
        m_Position = position;
        return m_Position;
    }

    const glm::vec3 & Camera::GetFront() const
    {
        return m_Front;
    }

    const glm::vec3 & Camera::GetUp() const
    {
        return m_Up;
    }

    const glm::vec3 Camera::SetUp( const glm::vec3 & up )
    {
        m_WorldUp = up;
        updateCameraVectors();
        return m_WorldUp;
    }

    float Camera::GetPitch() const
    {
        return m_Pitch;
    }

    const float Camera::SetPitch( float pitch )
    {
        m_Pitch = pitch;
        updateCameraVectors();
        return m_Pitch;
    }

    float Camera::GetYaw() const
    {
        return m_Yaw;
    }

    const float Camera::SetYaw( float yaw )
    {
        m_Yaw = yaw;
        updateCameraVectors();
        return m_Yaw;
    }

    float Camera::GetZoom() const
    {
        return m_Zoom;
    }

    const float Camera::SetZoom( float zoom )
    {
        m_Zoom = zoom;
        if ( m_Zoom < 1.0f )
        {
            m_Zoom = 1.0f;
        }
        if ( m_Zoom > 45.0f )
        {
            m_Zoom = 45.0f;
        }
        return m_Zoom;
    }
}
