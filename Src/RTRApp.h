//-----------------------------------------------------------------------------
// RTRApp.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>

class RTRApp
{
public:
    RTRApp(const char* title, bool fullscreen = false, int width = 1024, int height = 768)
        : m_MainWindowTitle{ title }
        , m_FullScreen{ fullscreen }
        , m_WindowWidth{ width }
        , m_WindowHeight{ height }
    { }
    virtual int Init();
    virtual void Done();
    void Run();
    virtual bool Tick() { return true; }

protected:
    std::string m_MainWindowTitle;
    bool m_FullScreen;
    int m_WindowWidth;
    int m_WindowHeight;
    int m_ScreenWidth{ 0 };
    int m_ScreenHeight{ 0 };
    SDL_Window* m_SDLWindow{ nullptr };
    SDL_Renderer* m_SDLRenderer{ nullptr };
    SDL_GLContext m_GLContext{ nullptr };
    unsigned int m_CurTime{ 0 };
    unsigned int m_TimeDelta{ 0 };
    int m_FPS{ 0 };
};
