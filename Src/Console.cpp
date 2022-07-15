//-----------------------------------------------------------------------------
// Console.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include <string>
#include <glad/glad.h>
#define GLT_IMPLEMENTATION
#include <gltext/gltext.h>
#include "Console.h"

#define MAX_STRING 1024
GLTtext* s_glt_text = nullptr;

void Console::Init()
{
    m_String = new char[MAX_STRING+1];
    gltInit();
    s_glt_text = gltCreateText();
}

void Console::End()
{
    gltDeleteText(s_glt_text);
    gltTerminate();
    delete m_String;
}

void Console::Render(const char *title, int fps, float pos_x, float pos_y, float pos_z, float pitch, float yaw, float tableRotation, float power)
{
    gltBeginDraw();

    sprintf_s(m_String, MAX_STRING, title);
    gltSetText(s_glt_text, m_String);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(s_glt_text, 10, 10, 2.0);

    sprintf_s(m_String, MAX_STRING,
        "\n"
        "Power: %.2f\n"
        , power);
    gltSetText(s_glt_text, m_String);
    gltColor(0.5f, 0.5f, 0.5f, 1.0f);
    gltDrawText2D(s_glt_text, 10, 10, 2.0);
    gltEndDraw();
    glUseProgram(0);
}

void Console::DebugRender(const char* title, int fps, float pos_x, float pos_y, float pos_z, float pitch, float yaw, float tableRotation, float power)
{
    gltBeginDraw();

    sprintf_s(m_String, MAX_STRING, title);
    gltSetText(s_glt_text, m_String);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(s_glt_text, 10, 10, 2.0);

    sprintf_s(m_String, MAX_STRING,
        "\n"
        "FPS: %d\n"
        "Cam Pos: %.2f, %.2f, %.2f\n"
        "Cam Yaw: %.2f\n"
        "Cam Pitch: %.2f\n"
        "Table Rotation: %.2f\n"
        "Power: %.2f\n"
        , fps, pos_x, pos_y, pos_z, yaw, pitch, tableRotation * 28.0f, power);
    gltSetText(s_glt_text, m_String);
    gltColor(0.5f, 0.5f, 0.5f, 1.0f);
    gltDrawText2D(s_glt_text, 10, 10, 2.0);
    gltEndDraw();
    glUseProgram(0);
}
