//-----------------------------------------------------------------------------
// Console.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once

class Console
{
public: 
    Console() {}
    ~Console() {}
    void Init();
    void End();
    void Render(const char *title, int fps, float pos_x, float pos_y, float pos_z, float yaw, float pitch, float tableRotation, float power);
    void DebugRender(const char* title, int fps, float pos_x, float pos_y, float pos_z, float pitch, float yaw, float tableRotation, float power);
private:
    char* m_String{ nullptr };
};
