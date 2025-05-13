#include <Input/InputHandler.h>

namespace Shinkiro::Input
{
    InputHandler & InputHandler::Get()
    {
        static InputHandler instance;
        return instance;
    }

    void InputHandler::Update()
    {
        m_previousKeyStates = m_currentKeyStates;

        // Update current key states
        for ( int i = 0; i < 256; i++ )
        {
            m_currentKeyStates[i] = ( GetAsyncKeyState( i ) & 0x8000 ) != 0;
        }
    }

    bool InputHandler::IsKeyPressed( int keyCode ) const
    {
        auto it = m_currentKeyStates.find( keyCode );
        return it != m_currentKeyStates.end() && it->second;
    }

    bool InputHandler::IsKeyJustPressed( int keyCode ) const
    {
        auto currIt = m_currentKeyStates.find( keyCode );
        auto prevIt = m_previousKeyStates.find( keyCode );

        bool currentState  = currIt != m_currentKeyStates.end() && currIt->second;
        bool previousState = prevIt != m_previousKeyStates.end() && prevIt->second;

        return currentState && !previousState;
    }

    bool InputHandler::IsKeyJustReleased( int keyCode ) const
    {
        auto currIt = m_currentKeyStates.find( keyCode );
        auto prevIt = m_previousKeyStates.find( keyCode );

        bool currentState  = currIt != m_currentKeyStates.end() && currIt->second;
        bool previousState = prevIt != m_previousKeyStates.end() && prevIt->second;

        return !currentState && previousState;
    }
}
