#pragma once

#ifndef SHINKIRO_RENDERER_CAMERA_H
#    define SHINKIRO_RENDERER_CAMERA_H

#    include <Renderer/Camera/Frustum.h>

#    include <glad/glad.h>
#    include <glm/glm.hpp>
#    include <glm/gtc/matrix_transform.hpp>

#    include <vector>

namespace Shinkiro::Renderer
{
    // Defines several possible options for camera movement.
    // Used as abstraction to stay away from window-system specific input methods
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // Default camera values
    const float YAW         = -90.0f;
    const float PITCH       = 0.0f;
    const float SPEED       = 5.0f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM        = 45.0f;

    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera
    {
    public:
        // Frustrum
        Frustum frustum;

        // camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        // euler Angles
        float Yaw;
        float Pitch;

        // camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

    public:
        // constructor with vectors
        Camera( glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f ), float yaw = YAW, float pitch = PITCH );

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard( Camera_Movement direction, float deltaTime );

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement( float xoffset, float yoffset, GLboolean constrainPitch = true );

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll( float yoffset );

        /**
         * @brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
         */
        glm::mat4 GetViewMatrix() const;

        /**
         * @brief Returns the projection matrix.
         * @param screenWidth The width of the viewport.
         * @param screenHeight The height of the viewport.
         * @return The perspective projection matrix.
         */
        glm::mat4 GetProjectionMatrix( int screenWidth, int screenHeight ) const;

        /**
         * @brief Calculates the mouse's position in world coordinates on the y=0 plane.
         * @param mouseX The current x-coordinate of the mouse.
         * @param mouseY The current y-coordinate of the mouse.
         * @param screenWidth The width of the viewport.
         * @param screenHeight The height of the viewport.
         * @return A 3D vector representing the mouse's position on the ground plane.
         */
        glm::vec3 GetMousePositionInWorld( float mouseX, float mouseY, int screenWidth, int screenHeight ) const;

        /**
         * @brief Updates the camera's front, right, and up vectors based on the current yaw and pitch angles.
         */
        void updateCameraVectors();

    public:
        // Getters for debug UI
        const glm::vec3 & GetPosition() const
        {
            return Position;
        }

        float GetYaw() const
        {
            return Yaw;
        }

        float GetPitch() const
        {
            return Pitch;
        }

    private:
        /**
         * @brief Calculates a world space ray from the camera through the specified screen coordinates.
         * @param mouseX The current x-coordinate of the mouse.
         * @param mouseY The current y-coordinate of the mouse.
         * @param screenWidth The width of the viewport.
         * @param screenHeight The height of the viewport.
         * @return A pair containing the ray's origin and direction vector.
         */
        std::pair<glm::vec3, glm::vec3> GetMouseRay( float mouseX, float mouseY, int screenWidth, int screenHeight ) const;
    };
}

#endif
