//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRLighting.h"
#include "RTRShader.h"

#include <vector>


class RTRCube;
class RTRSphere;
class RTRFlipper;
class RTRCylinder;

struct RTRPoint_t {
    float x, y, z;
    float normX, normY, normZ;
    float TexCoordX, TexCoordY;
};

struct RTRFace_t {
    unsigned int v0, v1, v2;
};

//-----------------------------------------------------------------------------
class RTRObject
{
public:
    RTRObject() {}
    ~RTRObject() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    void SetMaterial(RTRMaterial_t mat) { m_Material = mat; }
    virtual const char* GetName() { return "RTRObject"; }


    std::vector<RTRObject*> GetAllChildren();

    //Collision functions
    virtual void CalculateBoundingVolume(glm::mat4 ParentTransform);
    virtual bool IsColliding(RTRObject* CollidingObject);
    virtual bool IsColliding(RTRSphere* CollidingSphere);
    virtual bool IsColliding(RTRCube* CollidingCube);
    virtual bool IsColliding(RTRCylinder* CollidingCylinder);
    virtual bool OverlapsInterval(glm::vec2 MinimumPoint, glm::vec2 MaximumPoint);
    void CheckForCollision(RTRObject* CollidingObject);
    virtual void CheckForCollision(RTRSphere* CollidingSphere);
    virtual void CheckForCollision(RTRCube* CollidingCube);
    virtual void CheckForCollision(RTRCylinder* CollidingCylinder);
public:
    unsigned int m_NumVertices{ 0 };
    unsigned int m_NumFaces{ 0 };
    RTRFace_t* m_Faces{ nullptr };
    RTRPoint_t* m_Vertices{ nullptr };
    RTRMaterial_t m_Material{ {0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 128.0 };
    RTRMaterial_t m_FlipperMaterial{ {1.0, 0.19225, 0.19225 }, { 1.0, 0.50754, 0.50754 }, { 1.0, 0.508273, 0.508273 }, 128.0 };

    unsigned int m_VertexBuffer{ 0 };
    unsigned int m_VertexArray{ 0 };
    unsigned int m_FaceElementBuffer{ 0 };

    glm::mat4 m_Transform{ 1.0 };
    glm::mat4 m_GlobalTransform{ 1.0f };
    std::vector<RTRObject*> m_ChildObjects;

    unsigned int indexCount{ 0 };

    bool IsDynamic = false;

    bool IgnoreCollision = false;

    std::string label;
};

//-----------------------------------------------------------------------------
class RTRCube : public RTRObject
{
public:
    RTRCube() : RTRObject() {}
    ~RTRCube() {}
    virtual void Init(float x, float y, float z);
    virtual const char* GetName() { return "RTRCube"; }

    //Collision functions
    virtual void CalculateBoundingVolume(glm::mat4 ParentTransform) override;
    virtual bool IsColliding(RTRSphere* CollidingSphere);
    virtual bool OverlapsInterval(glm::vec2 MinimumPoint, glm::vec2 MaximumPoint) override;
    virtual void CheckForCollision(RTRSphere* CollidingSphere) override;
   
    float ScaleX, ScaleY, ScaleZ;

    std::vector<glm::vec2> SquarePoints;
    std::vector<glm::vec2> SquarePointNormals;
    glm::vec2 boundingBoxMin = glm::vec2(0.0f, 0.0f);
    glm::vec2 boundingBoxMax = glm::vec2(0.0f, 0.0f);
};

class RTRFlipper : public RTRCube
{
public:
    RTRFlipper() : RTRCube() {}
    ~RTRFlipper() {}
    virtual const char* GetName() { return "RTRFlipper"; }
    virtual void CalculateBoundingVolume(glm::mat4 ParentTransform) override;

    float mass = 2.0f;
    float RotationSpeed = 0.0f;
    glm::vec2 CentreOfRotation = glm::vec2(0.0f, 0.0f);
    float bounciness = 0.0f;
};

class RTRSphere : public RTRObject
{
public:
    RTRSphere() : RTRObject() {}
    ~RTRSphere() {}
    virtual void Init(float radius);
    void CalculateBoundingVolume(glm::mat4 ParentTransform);
    virtual const char* GetName() { return "RTRSphere"; }

    //Collision functions
    virtual bool IsColliding(RTRSphere* CollidingSphere) override;
    virtual bool IsColliding(RTRCube* CollidingCube) override;
    virtual bool IsColliding(RTRCylinder* CollidingCylinder) override;
    bool OverlapsInterval(glm::vec2 MinimumPoint, glm::vec2 MaximumPoint);
    virtual void CheckForCollision(RTRSphere* CollidingSphere) override;
    virtual void CheckForCollision(RTRCylinder* CollidingCylinder) override;
    virtual void CheckForCollision(RTRCube* CollidingCube) override;
    void PerformCollision(RTRSphere* CollidingSphere);
    void PerformCollision(RTRCylinder* CollidingCylinder);
    void PerformCollision(glm::vec2 IntersectedLine, glm::vec2 IntersectedPoint);
    void PerformCollision(glm::vec2 IntersectedLine, glm::vec2 IntersectedPoint, RTRFlipper* CollidingFlipper);

    bool SphereRayIntersection(glm::vec2 PointOne, glm::vec2 PointTwo, glm::vec2& IntersectionPoint, float& OverlapLength);

    glm::vec2 CentrePoint = glm::vec2(0.0f, 0.0f);
    float Radius = 0.0f;

    //Physics variables
    const float mass = 1.0f;
    glm::vec2 Velocity = glm::vec2(0.0f, 0.0f);

    int CollisionCount = 0;
};

class RTRCylinder : public RTRObject
{
public:
    RTRCylinder() : RTRObject() {}
    ~RTRCylinder() {}
    virtual void Init(float radius);
    void CalculateBoundingVolume(glm::mat4 ParentTransform);
    virtual const char* GetName() { return "RTRCylinder"; }
    virtual bool IsColliding(RTRSphere* CollidingSphere);
    virtual bool OverlapsInterval(glm::vec2 MinimumPoint, glm::vec2 MaximumPoint) override;
    virtual void CheckForCollision(RTRSphere* CollidingSphere) override;

    //Collision functions
    glm::vec2 CentrePoint = glm::vec2(0.0f, 0.0f);
    float Radius = 0.0f;

    //Physics variables
    const float mass = 2.0f;

    int CollisionCount = 0;
    float movement = -7;
};