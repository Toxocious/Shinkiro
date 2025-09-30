#pragma once

#ifndef SHINKIRO_RENDERER_CAMERA_H
#    define SHINKIRO_RENDERER_CAMERA_H

#    include <Renderer/_Defs.h>

#    include <Renderer/Camera/Frustum.h>

#    include <Core/Interfaces/CameraInterface.h>

#    include <glad/glad.h>
#    include <glm/glm.hpp>
#    include <glm/gtc/matrix_transform.hpp>

#    include <vector>

namespace Shinkiro::Renderer
{
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const float YAW         = -90.0f;
    const float PITCH       = 0.0f;
    const float SPEED       = 5.0f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM        = 45.0f;

    /**
     * @brief Camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
     */
    class RENDER_API Camera : public ICamera
    {
    public:
        /**
         * @brief Constructor with vectors
         * @param position The initial position of the camera
         * @param up The world up vector
         * @param yaw The initial yaw angle
         * @param pitch The initial pitch angle
         */
        Camera( glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f ), float yaw = YAW, float pitch = PITCH );

        ~Camera() override;

        /**
         * @brief Processes input received from any keyboard-like input system
         * @param direction The direction of movement
         * @param deltaTime The time difference between the current and last frame
         */
        void ProcessKeyboard( Camera_Movement direction, float deltaTime ) override;

        /**
         * @brief Processes input received from a mouse input system
         * @param xoffset The offset in the x direction
         * @param yoffset The offset in the y direction
         * @param constrainPitch Whether to constrain the pitch angle to prevent screen flipping
         */
        void ProcessMouseMovement( float xoffset, float yoffset, bool constrainPitch = true ) override;

        /**
         * @brief Processes input received from a mouse scroll-wheel event
         * @param yoffset The offset in the y direction (scroll amount)
         */
        void ProcessMouseScroll( float yoffset );

        /**
         * @brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
         */
        glm::mat4 GetViewMatrix() const;

        /**
         * @brief Returns the projection matrix
         * @param screenWidth The width of the viewport
         * @param screenHeight The height of the viewport
         * @return The perspective projection matrix
         */
        glm::mat4 GetProjectionMatrix( int screenWidth, int screenHeight ) const;

        /**
         * @brief Calculates the mouse's position in world coordinates on the y=0 plane
         * @param mouseX The current x-coordinate of the mouse
         * @param mouseY The current y-coordinate of the mouse
         * @param screenWidth The width of the viewport
         * @param screenHeight The height of the viewport
         * @return A 3D vector representing the mouse's position on the ground plane
         */
        glm::vec3 GetMousePositionInWorld( float mouseX, float mouseY, int screenWidth, int screenHeight ) const;

        /**
         * @brief Updates the camera's front, right, and up vectors based on the current yaw and pitch angles
         */
        void updateCameraVectors() override;

    private:
        /**
         * @brief Calculates a world space ray from the camera through the specified screen coordinates
         * @param mouseX The current x-coordinate of the mouse
         * @param mouseY The current y-coordinate of the mouse
         * @param screenWidth The width of the viewport
         * @param screenHeight The height of the viewport
         * @return A pair containing the ray's origin and direction vector
         */
        std::pair<glm::vec3, glm::vec3> GetMouseRay( float mouseX, float mouseY, int screenWidth, int screenHeight ) const;

    public:
        const Frustum & GetFrustum() const;

        const float GetMovementSpeed() const;

        const glm::vec3 & GetFront() const;
        float             GetPitch() const override;
        const glm::vec3 & GetPosition() const override;
        const glm::vec3 & GetUp() const;
        float             GetYaw() const override;
        float             GetZoom() const override;

        const glm::mat4 SetProjection( const glm::mat4 & projection ) override;
        const glm::vec3 SetPosition( const glm::vec3 & position ) override;
        const glm::vec3 SetUp( const glm::vec3 & up ) override;
        const float     SetPitch( float pitch ) override;
        const float     SetYaw( float yaw ) override;
        const float     SetZoom( float zoom ) override;

    private:
        // Frustrum
        Frustum m_Frustum;

        // Camera Attributes
        glm::mat4 m_Projection;
        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp;

        // Euler Angles
        float m_Yaw;
        float m_Pitch;

        // Camera Options
        float m_MovementSpeed;
        float m_MouseSensitivity;
        float m_Zoom;
    };
}

#endif
