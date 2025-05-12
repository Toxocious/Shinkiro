#pragma once

#include <Windows.h>
#include <unordered_map>

#ifdef PLATFORM_WINDOWS
#    ifdef SHINKIRO_INPUT_EXPORTS
#        define INPUT_API __declspec( dllexport )
#    else
#        define INPUT_API __declspec( dllimport )
#    endif
#else
#    define INPUT_API
#endif

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
