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
    }

    bool InputHandler::IsKeyPressed( int keyCode ) const
    {
        auto it = m_CurrentKeyStates.find( keyCode );
        if ( it == m_CurrentKeyStates.end() )
        {
            return false;
        }

        SHNK_CORE_TRACE( "Checking if key {} is held down.", keyCode );
        return it->second;
    }

    bool InputHandler::IsKeyJustPressed( int keyCode ) const
    {
        auto currIt = m_CurrentKeyStates.find( keyCode );
        if ( currIt == m_CurrentKeyStates.end() )
        {
            return false;
        }

        SHNK_CORE_TRACE( "Checking if key {} was pressed once.", keyCode );
        auto prevIt = m_PreviousKeyStates.find( keyCode );

        bool currentState  = currIt->second;
        bool previousState = prevIt != m_PreviousKeyStates.end() && prevIt->second;

        return currentState && !previousState;
    }

    bool InputHandler::IsKeyJustReleased( int keyCode ) const
    {
        auto currIt = m_CurrentKeyStates.find( keyCode );
        auto prevIt = m_PreviousKeyStates.find( keyCode );

        // A key can be "just released" even if it's not in the current map (if it was in the previous one).
        // So we check if it was present in the previous state.
        if ( prevIt == m_PreviousKeyStates.end() )
        {
            return false;
        }

        SHNK_CORE_TRACE( "Checking if key {} was released.", keyCode );

        bool currentState  = currIt != m_CurrentKeyStates.end() && currIt->second;
        bool previousState = prevIt->second;

        return !currentState && previousState;
    }

    bool InputHandler::IsMouseButtonPressed( int button ) const
    {
        if ( m_CurrentKeyStates.count( button ) )
        {
            SHNK_CORE_TRACE( "Checking if mouse button {} is held down.", button );
        }
        return IsKeyPressed( button );
    }

    bool InputHandler::IsMouseButtonJustPressed( int button ) const
    {
        if ( m_CurrentKeyStates.count( button ) )
        {
            SHNK_CORE_TRACE( "Checking if mouse button {} was pressed once.", button );
        }
        return IsKeyJustPressed( button );
    }

    bool InputHandler::IsMouseButtonJustReleased( int button ) const
    {
        if ( m_PreviousKeyStates.count( button ) )
        {
            SHNK_CORE_TRACE( "Checking if mouse button {} was released.", button );
        }
        return IsKeyJustReleased( button );
    }

    void InputHandler::KeyCallback( int key, int scancode, int action, int mods )
    {
        if ( action == GLFW_PRESS )
        {
            m_CurrentKeyStates[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            m_CurrentKeyStates[key] = false;
        }
    }

    void InputHandler::MouseButtonCallback( int button, int action, int mods )
    {
        if ( action == GLFW_PRESS )
        {
            m_CurrentKeyStates[button] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            m_CurrentKeyStates[button] = false;
        }
    }
}
