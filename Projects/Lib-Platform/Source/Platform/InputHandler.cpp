#include <Platform/_Common.h>

#include <Platform/InputHandler.h>

namespace Shinkiro::Platform
{
    InputHandler & InputHandler::Get()
    {
        static InputHandler instance;
        return instance;
    }

    void InputHandler::Update()
    {
        m_PreviousKeyStates = m_CurrentKeyStates;

        for ( int i = 0; i < 256; i++ )
        {
            m_CurrentKeyStates[i] = ( GetAsyncKeyState( i ) & 0x8000 ) != 0;
        }
    }

    bool InputHandler::IsKeyPressed( int keyCode ) const
    {
        auto it = m_CurrentKeyStates.find( keyCode );
        return it != m_CurrentKeyStates.end() && it->second;
    }

    bool InputHandler::IsKeyJustPressed( int keyCode ) const
    {
        auto currIt = m_CurrentKeyStates.find( keyCode );
        auto prevIt = m_PreviousKeyStates.find( keyCode );

        bool currentState  = currIt != m_CurrentKeyStates.end() && currIt->second;
        bool previousState = prevIt != m_PreviousKeyStates.end() && prevIt->second;

        return currentState && !previousState;
    }

    bool InputHandler::IsKeyJustReleased( int keyCode ) const
    {
        auto currIt = m_CurrentKeyStates.find( keyCode );
        auto prevIt = m_PreviousKeyStates.find( keyCode );

        bool currentState  = currIt != m_CurrentKeyStates.end() && currIt->second;
        bool previousState = prevIt != m_PreviousKeyStates.end() && prevIt->second;

        return !currentState && previousState;
    }
}
