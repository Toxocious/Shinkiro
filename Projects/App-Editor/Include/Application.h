#pragma once

// OS Headers
#include <Windows.h>

// Projects/Lib-* Headers
#include <Audio/AudioHandler.h>
#include <Input/InputHandler.h>
#include <Renderer/RenderHandler.h>

class Application
{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    HWND m_hwnd;
    bool m_running;

    const wchar_t * m_windowClassName = L"EditorWindowClass";
    const wchar_t * m_windowTitle     = L"Shinkiro Editor";
    const int       m_windowWidth     = 1280;
    const int       m_windowHeight    = 720;

    // Audio Manager
    Shinkiro::Audio::AudioHandler * m_AudioHandler;
};
