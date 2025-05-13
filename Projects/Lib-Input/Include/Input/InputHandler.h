#pragma once

#ifndef SHINKIRO_INPUT_INPUTHANDLER_H
#    define SHINKIRO_INPUT_INPUTHANDLER_H

#    include <Input/_Defs.h>

#    include <Windows.h>
#    include <unordered_map>

namespace Shinkiro::Input
{
    class INPUT_API InputHandler
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
