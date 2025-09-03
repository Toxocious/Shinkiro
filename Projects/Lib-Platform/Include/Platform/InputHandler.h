#pragma once

#ifndef SHINKIRO_PLATFORM_INPUTHANDLER_H
#    define SHINKIRO_PLATFORM_INPUTHANDLER_H

#    include <Platform/_Defs.h>

#    include <unordered_map>

namespace Shinkiro::Platform
{
    class PLATFORM_API InputHandler
    {
    public:
        /**
         * @brief Get the singleton instance of the InputHandler
         */
        static InputHandler & Get();

        /**
         * @brief Update the input states. This should be called once per frame
         */
        void Update();

        /**
         * @brief Check if a key is currently pressed
         * @param keyCode The key code to check
         * @return True if the key is pressed, false otherwise
         */
        bool IsKeyPressed( int keyCode ) const;

        /**
         * @brief Check if a key was just pressed this frame
         * @param keyCode The key code to check
         * @return True if the key was just pressed, false otherwise
         */
        bool IsKeyJustPressed( int keyCode ) const;

        /**
         * @brief Check if a key was just released this frame
         * @param keyCode The key code to check
         * @return True if the key was just released, false otherwise
         */
        bool IsKeyJustReleased( int keyCode ) const;

        /**
         * @brief Check if a mouse button is currently pressed
         * @param button The mouse button code to check (e.g., GLFW_MOUSE_BUTTON_LEFT)
         * @return True if the button is pressed, false otherwise
         */
        bool IsMouseButtonPressed( int button ) const;

        /**
         * @brief Check if a mouse button was just pressed this frame
         * @param button The mouse button code to check
         * @return True if the button was just pressed, false otherwise
         */
        bool IsMouseButtonJustPressed( int button ) const;

        /**
         * @brief Check if a mouse button was just released this frame
         * @param button The mouse button code to check
         * @return True if the button was just released, false otherwise
         */
        bool IsMouseButtonJustReleased( int button ) const;

        /**
         * @brief Callback for key events
         * @param key The key code
         * @param scancode The system-specific scancode
         * @param action The action (press, release, repeat)
         * @param mods Modifier keys (shift, ctrl, alt)
         */
        void KeyCallback( int key, int scancode, int action, int mods );

        /**
         * @brief Callback for mouse button events
         * @param button The mouse button
         * @param action The action (press, release)
         * @param mods Modifier keys (shift, ctrl, alt)
         */
        void MouseButtonCallback( int button, int action, int mods );

    private:
        InputHandler()                                   = default;
        ~InputHandler()                                  = default;
        InputHandler( const InputHandler & )             = delete;
        InputHandler & operator=( const InputHandler & ) = delete;

    private:
        std::unordered_map<int, bool> m_CurrentKeyStates;
        std::unordered_map<int, bool> m_PreviousKeyStates;
    };
}

#endif
