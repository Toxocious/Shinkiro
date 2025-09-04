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

        return it->second;
    }

    bool InputHandler::IsKeyJustPressed( int keyCode ) const
    {
        auto currIt = m_CurrentKeyStates.find( keyCode );
        if ( currIt == m_CurrentKeyStates.end() )
        {
            return false;
        }

        auto prevIt = m_PreviousKeyStates.find( keyCode );

        bool currentState  = currIt->second;
        bool previousState = prevIt != m_PreviousKeyStates.end() && prevIt->second;

        return currentState && !previousState;
    }

    bool InputHandler::IsKeyJustReleased( int keyCode ) const
    {
        auto currIt = m_CurrentKeyStates.find( keyCode );
        auto prevIt = m_PreviousKeyStates.find( keyCode );

        if ( prevIt == m_PreviousKeyStates.end() )
        {
            return false;
        }

        bool currentState  = currIt != m_CurrentKeyStates.end() && currIt->second;
        bool previousState = prevIt->second;

        return !currentState && previousState;
    }

    bool InputHandler::IsMouseButtonPressed( int button ) const
    {
        if ( m_CurrentKeyStates.count( button ) )
        {
        }
        return IsKeyPressed( button );
    }

    bool InputHandler::IsMouseButtonJustPressed( int button ) const
    {
        if ( m_CurrentKeyStates.count( button ) )
        {
        }
        return IsKeyJustPressed( button );
    }

    bool InputHandler::IsMouseButtonJustReleased( int button ) const
    {
        if ( m_PreviousKeyStates.count( button ) )
        {
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
