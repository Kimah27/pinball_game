//-----------------------------------------------------------------------------
// MainApp.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRLighting.h"
#include "RTRObject.h"
#include "Console.h"

struct UniformGridCell {
    glm::vec2 MinimumPoint;
    glm::vec2 MaximumPoint;
    std::vector<RTRObject*> OverlappingObjects;
};

class MainApp : public RTRApp
{
public:
    MainApp(const char* title, bool fullscreen = false, int width = 1024, int height = 768)
        : RTRApp(title, fullscreen, width, height) 
    { }
    int Init();
    void Done();
    bool Tick();

    void DrawObject(RTRObject* object, glm::mat4 parentTransform, RTRShader* shader);

private:
    bool m_QuitApp{ false };
    RTRShader* m_DefaultShader{ nullptr };
    RTRCamera* m_Camera{ nullptr };
    RTRLightingModel* m_LightingModel{ nullptr };
    Console* m_Console{ nullptr };

    unsigned int texture;
    unsigned int texture2;

    unsigned int skybox;
    unsigned int m_VertexArray{ 0 };

    RTRObject* table{ nullptr };
    RTRCube* base{ nullptr };
    RTRCube* topWall{ nullptr };
    RTRCube* bottomWall{ nullptr };
    RTRCube* leftWall{ nullptr };
    RTRCube* rightWall{ nullptr };
    RTRCube* inLane{ nullptr };
    RTRCube* cornerWedge{ nullptr };
    RTRCube* rampL{ nullptr };
    RTRCube* rampL2{ nullptr };
    //RTRCube* rampR{ nullptr };
    RTRFlipper* rampR{ nullptr };
    RTRCube* rampR2{ nullptr };

    RTRFlipper* bumper{ nullptr };
    RTRFlipper* plunger{ nullptr };
    
    RTRObject* flipperLParent{ nullptr };
    RTRObject* flipperRParent{ nullptr };

    RTRFlipper* flipperL{ nullptr };
    RTRFlipper* flipperR{ nullptr };

    RTRObject* bumperCircleParent{ nullptr };
    RTRCylinder* bumperCircle{ nullptr };

    RTRSphere* ball{ nullptr };

    std::vector<RTRSphere*> balls;
    int ballCount = 0;

    bool m_MovingForward{ false };
    bool m_MovingBackward{ false };
    bool m_TurningLeft{ false };
    bool m_TurningRight{ false };
    bool m_StrafingLeft{ false };
    bool m_StrafingRight{ false };
    bool m_TiltingUp{ false };
    bool m_TiltingDown{ false };

    bool m_BoardTiltDown{ false };
    bool m_BoardTiltUp{ false };

    bool m_LeftFlipper{ false };
    bool m_RightFlipper{ false };

    bool m_LaunchBall{ false };
    bool m_HUDDisplay{ false };
    bool toggleHUD{ false };

    glm::mat4 m_ModelMatrix{ 1.0 };
    glm::mat4 m_ViewMatrix{ 1.0 };
    glm::mat4 m_ProjectionMatrix{ 1.0 };

    void CheckInput();
    void UpdateState(unsigned int td_milli);
    void UpdatePhysics(unsigned int td_milli);
    glm::vec2 ProjectVectorOntoAxis(glm::vec2 Vector, glm::vec2 Axis);
    void UpdateUniformGrid(unsigned int td_milli);
    void CheckCollisions(unsigned int td_milli);
    void RenderFrame();

    std::vector<RTRObject*> DynamicObjects;

    //Uniform Grid Variables
    std::vector<UniformGridCell> UniformGrid;
    float IntervalSize = 1.1f;
    int RowsOfCells = 10;
    int ColumnsOfCells = 6;

    void InitialiseUniformGrid();

    //Physics variables
    float Gravity = 1.1f;
    float TableAngle = 0.3f;

    float power = 0.0f;
    float launch = 0.0f;
    float lFlipperAngle = 0.0f;
    float rFlipperAngle = 0.0f;
    float FlipperRotationSpeed = 800.0f;

};
