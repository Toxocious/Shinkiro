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
