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
        static InputHandler & Get();

        void Update();
        bool IsKeyPressed( int keyCode ) const;
        bool IsKeyJustPressed( int keyCode ) const;
        bool IsKeyJustReleased( int keyCode ) const;

    private:
        InputHandler()                                   = default;
        ~InputHandler()                                  = default;
        InputHandler( const InputHandler & )             = delete;
        InputHandler & operator=( const InputHandler & ) = delete;

        std::unordered_map<int, bool> m_currentKeyStates;
        std::unordered_map<int, bool> m_previousKeyStates;
    };
}

#endif
