#include "Application.h"

Application::Application()
    : m_hwnd( nullptr ), m_running( false )
{
}

Application::~Application()
{
    Shutdown();
}

LRESULT CALLBACK Application::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }
    return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

bool Application::Initialize()
{
    WNDCLASSEX wc    = {};
    wc.cbSize        = sizeof( WNDCLASSEX );
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = GetModuleHandle( nullptr );
    wc.lpszClassName = m_windowClassName;
    wc.hCursor       = LoadCursor( nullptr, IDC_ARROW );

    if ( !RegisterClassEx( &wc ) )
    {
        return false;
    }

    RECT windowRect = { 0, 0, m_windowWidth, m_windowHeight };
    AdjustWindowRect( &windowRect, WS_OVERLAPPEDWINDOW, FALSE );

    m_hwnd = CreateWindowEx(
        0,
        m_windowClassName,
        m_windowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        GetModuleHandle( nullptr ),
        nullptr
    );

    if ( !m_hwnd )
    {
        return false;
    }

    // setup audio handler
    m_AudioHandler = new Shinkiro::Audio::AudioHandler();

    ShowWindow( m_hwnd, SW_SHOW );
    m_running = true;
    return true;
}

void Application::Run()
{
    MSG msg = {};
    while ( m_running )
    {
        while ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            if ( msg.message == WM_QUIT )
            {
                m_running = false;
                break;
            }
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }
}

void Application::Shutdown()
{
    if ( m_hwnd )
    {
        DestroyWindow( m_hwnd );
        m_hwnd = nullptr;
    }
    UnregisterClass( m_windowClassName, GetModuleHandle( nullptr ) );
}
