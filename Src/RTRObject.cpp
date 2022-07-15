//-----------------------------------------------------------------------------
// RTRObject.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include <iostream>
#include "RTRObject.h"
#include <glm/gtx/matrix_decompose.hpp>

// README / TODO - This is just a simple start.
// * For your assignment, think carefully about how Shaders, Materials and Objects will hang together.
// * Have a hierarchy of differnet object types (static object, moving object, hierarchical object, object with force, etc...
// * Don't worry too much about optimising the buffer handling by, for example, merging all objects into a single 
// vertex buffer. Or sorting object based on shader or material to minimize switches. Spend your time on more imprtant 
// optimisation techniques that will add marks - for example your uniform grid implementation. 
// * For this assignment you can hardcode the vertex buffers for the differnet shapes (see cube) and then 
// scale, place and animate them with transformation matrices.

//-----------------------------------------------------------------------------
void RTRObject::Init()
{
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t), m_Vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);
}

void RTRObject::Render(RTRShader *shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, m_NumFaces * 3, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, m_NumVertices);
    glBindVertexArray(0);
}

void RTRObject::End()
{
    glDeleteVertexArrays(1, &m_VertexArray); m_VertexArray = 0;
    glDeleteBuffers(1, &m_VertexBuffer); m_VertexBuffer = 0;
    glDeleteBuffers(1, &m_FaceElementBuffer); m_FaceElementBuffer = 0;
    if (m_Faces != nullptr) { delete m_Faces; m_Faces = nullptr; }
    if (m_Vertices != nullptr) { delete m_Vertices; m_Vertices = nullptr; }
}

std::vector<RTRObject*> RTRObject::GetAllChildren()
{
    std::vector<RTRObject*> ReturnArray = std::vector<RTRObject*>();
    ReturnArray.push_back(this);

    for (RTRObject* child : m_ChildObjects)
    {
        std::vector<RTRObject*> ChildList = child->GetAllChildren();
        for (RTRObject* c : ChildList)
        {
            ReturnArray.push_back(c);
        }
    }

    return ReturnArray;
}

void RTRObject::CalculateBoundingVolume(glm::mat4 ParentTransform)
{
    for (RTRObject* obj : m_ChildObjects)
    {
        if (label != "Table")
        {
            obj->CalculateBoundingVolume(ParentTransform * m_Transform);
        }
        else
        {
            obj->CalculateBoundingVolume(ParentTransform);
        }
    }
}

bool RTRObject::IsColliding(RTRObject* CollidingObject)
{
    if (CollidingObject->GetName() == "RTRSphere")
    {
        RTRSphere* Sphere = (RTRSphere*)CollidingObject;
        return IsColliding(Sphere);
    }
    else if (CollidingObject->GetName() == "RTRCube" || CollidingObject->GetName() == "RTRFlipper")
    {
        RTRCube* Cube = (RTRCube*)CollidingObject;
        return IsColliding(Cube);
    }
    else if (CollidingObject->GetName() == "RTRCylinder")
    {
        RTRCylinder* Cylinder = (RTRCylinder*)CollidingObject;
        return IsColliding(Cylinder);
    }
    return false;
}

bool RTRObject::IsColliding(RTRSphere* CollidingSphere)
{
    return false;
}

bool RTRObject::IsColliding(RTRCube* CollidingCube)
{
    return false;
}

bool RTRObject::IsColliding(RTRCylinder* CollidingCylinder)
{
    return false;
}

bool RTRObject::OverlapsInterval(glm::vec2 MinimumPoint, glm::vec2 MaximumPoint)
{
    return false;
}

void RTRObject::CheckForCollision(RTRObject* CollidingObject)
{
    if (CollidingObject->GetName() == "RTRSphere")
    {
        RTRSphere* Sphere = (RTRSphere*)CollidingObject;
        return CheckForCollision(Sphere);
    }
    else if (CollidingObject->GetName() == "RTRCube")
    {
        RTRCube* Cube = (RTRCube*)CollidingObject;
        return CheckForCollision(Cube);
    }
    else if (CollidingObject->GetName() == "RTRCylinder")
    {
        RTRCylinder* Cylinder = (RTRCylinder*)CollidingObject;
        return CheckForCollision(Cylinder);
    }
}

void RTRObject::CheckForCollision(RTRSphere* CollidingSphere)
{
}

void RTRObject::CheckForCollision(RTRCube* CollidingCube)
{
}

void RTRObject::CheckForCollision(RTRCylinder* CollidingCylinder)
{
}

//------------------------------------------------------------------------
void RTRCube::Init(float x, float y, float z)
{
    m_NumVertices = 36;
    m_NumFaces = 12;

    ScaleX = x;
    ScaleY = y;
    ScaleZ = z;

    m_Faces = new RTRFace_t[]
    {
        {  0,  2,  1 }, {  3,  5,  4 },  // -z
        {  6,  7,  8 }, {  9, 10, 11 },  // +z
        { 12, 13, 14 }, { 15, 16, 17 },  // -x
        { 18, 20, 19 }, { 21, 23, 22 },  // +x
        { 24, 25, 26 }, { 27, 28, 29 },  // -y
        { 30, 32, 31 }, { 33, 35, 34 }   // +y
    };

    m_Vertices = new RTRPoint_t[]
    {
        // positions   // normals              // texture coords
        -x, -y, -z,     0.0f,  0.0f, -1.0f,    0,  0,
         x, -y, -z,     0.0f,  0.0f, -1.0f,    x,  0,
         x,  y, -z,     0.0f,  0.0f, -1.0f,    x,  y,
         x,  y, -z,     0.0f,  0.0f, -1.0f,    x,  y,
        -x,  y, -z,     0.0f,  0.0f, -1.0f,    0,  y,
        -x, -y, -z,     0.0f,  0.0f, -1.0f,    0,  0,
                                          
        -x, -y,  z,     0.0f,  0.0f,  1.0f,    0,  0,
         x, -y,  z,     0.0f,  0.0f,  1.0f,    x,  0,
         x,  y,  z,     0.0f,  0.0f,  1.0f,    x,  y,
         x,  y,  z,     0.0f,  0.0f,  1.0f,    x,  y,
        -x,  y,  z,     0.0f,  0.0f,  1.0f,    0,  y,
        -x, -y,  z,     0.0f,  0.0f,  1.0f,    0,  0,
                                         
        -x,  y,  z,    -1.0f,  0.0f,  0.0f,    y,  0,
        -x,  y, -z,    -1.0f,  0.0f,  0.0f,    y,  z,
        -x, -y, -z,    -1.0f,  0.0f,  0.0f,    0,  z,
        -x, -y, -z,    -1.0f,  0.0f,  0.0f,    0,  z,
        -x, -y,  z,    -1.0f,  0.0f,  0.0f,    0,  0,
        -x,  y,  z,    -1.0f,  0.0f,  0.0f,    y,  0,
                                        
         x,  y,  z,     1.0f,  0.0f,  0.0f,    y,  0,
         x,  y, -z,     1.0f,  0.0f,  0.0f,    y,  z,
         x, -y, -z,     1.0f,  0.0f,  0.0f,    0,  z,
         x, -y, -z,     1.0f,  0.0f,  0.0f,    0,  z,
         x, -y,  z,     1.0f,  0.0f,  0.0f,    0,  0,
         x,  y,  z,     1.0f,  0.0f,  0.0f,    y,  0,
                                        
        -x, -y, -z,     0.0f, -1.0f,  0.0f,    0,  z,
         x, -y, -z,     0.0f, -1.0f,  0.0f,    x,  z,
         x, -y,  z,     0.0f, -1.0f,  0.0f,    x,  0,
         x, -y,  z,     0.0f, -1.0f,  0.0f,    x,  0,
        -x, -y,  z,     0.0f, -1.0f,  0.0f,    0,  0,
        -x, -y, -z,     0.0f, -1.0f,  0.0f,    0,  z,
                                       
        -x,  y, -z,     0.0f,  1.0f,  0.0f,    0,  z,
         x,  y, -z,     0.0f,  1.0f,  0.0f,    x,  z,
         x,  y,  z,     0.0f,  1.0f,  0.0f,    x,  0,
         x,  y,  z,     0.0f,  1.0f,  0.0f,    x,  0,
        -x,  y,  z,     0.0f,  1.0f,  0.0f,    0,  0,
        -x,  y, -z,     0.0f,  1.0f,  0.0f,    0,  z
    };                                      
                                            
    RTRObject::Init();                      
}

void RTRCube::CalculateBoundingVolume(glm::mat4 ParentTransform)
{


    for (int i = 0; i < m_NumVertices; i++)
    {
        glm::vec4 Vertice = glm::vec4(m_Vertices[i].x, m_Vertices[i].y, m_Vertices[i].z, 1.0f);
        glm::vec4 TransformedPoint = ParentTransform * m_Transform * Vertice;

        if (i == 0)
        {
            boundingBoxMin.x = TransformedPoint.x;
            boundingBoxMin.y = TransformedPoint.z;
            boundingBoxMax.x = TransformedPoint.x;
            boundingBoxMax.y = TransformedPoint.z;
        }
        else if (TransformedPoint.y > 0.0f)
        {
            if (TransformedPoint.x < boundingBoxMin.x)
            {
                boundingBoxMin.x = TransformedPoint.x;
            }
            if (TransformedPoint.x > boundingBoxMax.x)
            {
                boundingBoxMax.x = TransformedPoint.x;
            }
            if (TransformedPoint.z < boundingBoxMin.y)
            {
                boundingBoxMin.y = TransformedPoint.z;
            }
            if (TransformedPoint.z > boundingBoxMax.y)
            {
                boundingBoxMax.y = TransformedPoint.z;
            }
        }
    }

    //Get all the points of the cube.
    SquarePoints.clear();

    glm::vec4 Vertice = glm::vec4(ScaleX, 0.0f, ScaleZ, 1.0f);
    glm::vec4 TransformedPoint = ParentTransform * m_Transform * Vertice;
    SquarePoints.push_back(glm::vec2(TransformedPoint.x, TransformedPoint.z));

    Vertice = glm::vec4(-ScaleX, 0.0f, ScaleZ, 1.0f);
    TransformedPoint = ParentTransform * m_Transform * Vertice;
    SquarePoints.push_back(glm::vec2(TransformedPoint.x, TransformedPoint.z));

    Vertice = glm::vec4(-ScaleX, 0.0f, -ScaleZ, 1.0f);
    TransformedPoint = ParentTransform * m_Transform * Vertice;
    SquarePoints.push_back(glm::vec2(TransformedPoint.x, TransformedPoint.z));

    Vertice = glm::vec4(ScaleX, 0.0f, -ScaleZ, 1.0f);
    TransformedPoint = ParentTransform * m_Transform * Vertice;
    SquarePoints.push_back(glm::vec2(TransformedPoint.x, TransformedPoint.z));

    SquarePointNormals.clear();
    for (int i = 0; i < 4; i++)
    {
        int j = i + 1;
        if(j == 4)
        {
            j = 0;
        }
        glm::vec2 Normal = glm::vec2((SquarePoints[i] - SquarePoints[j]).y, -(SquarePoints[i] - SquarePoints[j]).x);
        Normal = glm::normalize(Normal);
        SquarePointNormals.push_back(Normal);
    }

    for (RTRObject* obj : m_ChildObjects)
    {
        obj->CalculateBoundingVolume(ParentTransform * m_Transform);
    }
}

bool RTRCube::IsColliding(RTRSphere* CollidingSphere)
{
    return CollidingSphere->IsColliding(this);
}

bool RTRCube::OverlapsInterval(glm::vec2 MinimumPoint, glm::vec2 MaximumPoint)
{
    /*
    intervalintersect(A,B)
    returns (OVERLAP,DISJOINT)
    1:  if (amin>bmaxorbmin>amax)
    2:    return (DISJOINT);
    3   else
    4:    return (OVERLAP);
    */

    if (MinimumPoint.x > boundingBoxMax.x || boundingBoxMin.x > MaximumPoint.x ||
        MinimumPoint.y > boundingBoxMax.y || boundingBoxMin.y > MaximumPoint.y)
    {
        return false;
    }
    else
    {
        return true;
    }

}

void RTRCube::CheckForCollision(RTRSphere* CollidingSphere)
{
    CollidingSphere->CheckForCollision(this);
}

void RTRSphere::Init(float radius)
{
    const unsigned int X_SEGMENTS = 32;
    const unsigned int Y_SEGMENTS = 32;
    const float PI = 3.14159265359f;
    
    m_NumVertices = (X_SEGMENTS + 1) * (Y_SEGMENTS + 1);
    m_NumFaces = X_SEGMENTS * 2 * (Y_SEGMENTS + 1);

    m_Faces = new RTRFace_t[m_NumFaces];

    Radius = radius;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    //Calculate all our points, uvs and normals
    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
    {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI) * radius;
            float yPos = std::cos(ySegment * PI) * radius;
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI) * radius;

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    //Calculate the face indices
    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x + 1);

                indices.push_back(y * (X_SEGMENTS + 1) + x);                
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x + 1);
                indices.push_back((y) * (X_SEGMENTS + 1) + x + 1);
            }
        }
    }
    indexCount = indices.size();

    for (size_t i = 0; i < indexCount / 3; i++)
    {
        m_Faces[i] = RTRFace_t(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);
    }

    m_Vertices = new RTRPoint_t[m_NumVertices];

    for (unsigned int i = 0; i < positions.size(); ++i)
    {
        RTRPoint_t point;
        point.x = (positions[i].x);
        point.y = (positions[i].y);
        point.z = (positions[i].z);
        if (normals.size() > 0)
        {
            point.normX = (normals[i].x);
            point.normY = (normals[i].y);
            point.normZ = (normals[i].z);
        }
        if (uv.size() > 0)
        {
            point.TexCoordX = (uv[i].x);
            point.TexCoordY = (uv[i].y);
        }
        
        m_Vertices[i] = point;
    } 

    RTRObject::Init();
}

void RTRSphere::CalculateBoundingVolume(glm::mat4 ParentTransform)
{
    //Find centre point
    glm::vec4 Origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 BallPosition = m_Transform * Origin;
    CentrePoint = glm::vec2(BallPosition.x, BallPosition.z);
}

bool RTRSphere::IsColliding(RTRSphere* CollidingSphere)
{
    /*
    l=c2−c1
    2:d2=l.l
    3:  if (d2<(r1+r2)2)
    4:    return (OVERLAP);
    5:  else6:    return (DISJOINT);
    */

    float dist = std::abs(CollidingSphere->CentrePoint.x - CentrePoint.x) + std::abs(CollidingSphere->CentrePoint.y - CentrePoint.y);
    float distSquared = dist * dist;

    if (distSquared < std::pow(CollidingSphere->Radius + Radius, 2.0f))
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool RTRSphere::OverlapsInterval(glm::vec2 MinimumPoint, glm::vec2 MaximumPoint)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_Transform, scale, rotation, translation, skew, perspective);

    float d = 0;

    //Check x axis
    if (translation.x < MinimumPoint.x)
    {
        d += std::pow((translation.x - MinimumPoint.x), 2.0f);
    }
    else if (translation.x > MaximumPoint.x)
    {
        d += std::pow((translation.x - MaximumPoint.x), 2.0f);
    }

    //Check y axis
    if (translation.z < MinimumPoint.y)
    {
        d += std::pow((translation.z - MinimumPoint.y), 2.0f);
    }
    else if (translation.z > MaximumPoint.y)
    {
        d += std::pow((translation.z - MaximumPoint.y), 2.0f);
    }

    //Check if distance from sphere is less than radius
    if (d < std::pow(Radius, 2.0f))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RTRSphere::CheckForCollision(RTRSphere* CollidingSphere)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_Transform, scale, rotation, translation, skew, perspective);
    glm::vec2 MyPosition = glm::vec2(translation.x, translation.z);

    glm::decompose(CollidingSphere->m_Transform, scale, rotation, translation, skew, perspective);
    glm::vec2 OtherBallPosition = glm::vec2(translation.x, translation.z);

    glm::vec2 NormalOfCollision = MyPosition - OtherBallPosition;
    float DistanceBetweenBalls = glm::length(NormalOfCollision);

    if (DistanceBetweenBalls < Radius + CollidingSphere->Radius)
    {
        PerformCollision(CollidingSphere);
    }
}

void RTRSphere::CheckForCollision(RTRCylinder* CollidingCylinder)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_Transform, scale, rotation, translation, skew, perspective);
    glm::vec2 MyPosition = glm::vec2(translation.x, translation.z);

    glm::decompose(CollidingCylinder->m_Transform, scale, rotation, translation, skew, perspective);
    glm::vec2 OtherBallPosition = glm::vec2(translation.x, translation.z);

    glm::vec2 NormalOfCollision = MyPosition - OtherBallPosition;
    float DistanceBetweenBalls = glm::length(NormalOfCollision);

    if (DistanceBetweenBalls < Radius + CollidingCylinder->Radius)
    {
        PerformCollision(CollidingCylinder);
    }
}

void RTRSphere::CheckForCollision(RTRCube* CollidingCube)
{
    glm::vec2 IntersectPoint = glm::vec2(0.0f, 0.0f);
    bool CollisionFound = false;
    glm::vec2 NormalOfCollision = glm::vec2(0.0f, 0.0f);
    float OverlapLength = 0.0f;
    for (int i = 0; i < CollidingCube->SquarePoints.size(); i++)
    {
        int j = i + 1;
        if (j == 4)
        {
            j = 0;
        }
        if (SphereRayIntersection(CollidingCube->SquarePoints[i], CollidingCube->SquarePoints[j], IntersectPoint, OverlapLength))
        {
            NormalOfCollision = CollidingCube->SquarePointNormals[i];
            if (CollidingCube->GetName() == "RTRFlipper")
            {
                RTRFlipper* flipper = (RTRFlipper *) CollidingCube;
                PerformCollision(NormalOfCollision, IntersectPoint, flipper);
            }
            else
            {
                PerformCollision(NormalOfCollision, IntersectPoint);
            }
        }
    }
    
    
}

void RTRSphere::PerformCollision(RTRSphere * CollidingSphere)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_Transform, scale, rotation, translation, skew, perspective);
    glm::vec2 MyPosition = glm::vec2(translation.x, translation.z);

    glm::decompose(CollidingSphere->m_Transform, scale, rotation, translation, skew, perspective);
    glm::vec2 OtherBallPosition = glm::vec2(translation.x, translation.z);

    glm::vec2 NormalOfCollision = MyPosition - OtherBallPosition;
    NormalOfCollision = glm::normalize(NormalOfCollision);

    //Velocity perpendicular to the collision
    glm::vec2 MyNormalVelocity = glm::dot(NormalOfCollision, Velocity) * NormalOfCollision;
    glm::vec2 OtherNormalVelocity = glm::dot(NormalOfCollision, CollidingSphere->Velocity) * NormalOfCollision;
    glm::vec2 MyTangentVelocity = Velocity - MyNormalVelocity;
    glm::vec2 OtherTangentVelocity = CollidingSphere->Velocity - OtherNormalVelocity;

    //Move balls to the point of initial collision to prevent getting stuck inside eachother.
    float CombinedRadius = Radius + CollidingSphere->Radius;
    float DistApart = glm::length(MyPosition - OtherBallPosition);

    float OverlapDistance = CombinedRadius - DistApart;
    if (OverlapDistance > 0.0f)
    {
        glm::vec2 TranslationVec = (OverlapDistance + 0.0001f) * NormalOfCollision / 2.0f;

        m_Transform = glm::translate(m_Transform, glm::vec3(TranslationVec.x, 0.0f, TranslationVec.y));
        CollidingSphere->m_Transform = glm::translate(CollidingSphere->m_Transform, -glm::vec3(TranslationVec.x, 0.0f, TranslationVec.y));
    }

    //Calculate the end velocities
    Velocity = (MyNormalVelocity * (mass - CollidingSphere->mass) / (mass + CollidingSphere->mass)) + (OtherNormalVelocity * (2 * CollidingSphere->mass) / (mass + CollidingSphere->mass)) + MyTangentVelocity;
    CollidingSphere->Velocity = (MyNormalVelocity * (2 * mass) / (mass + CollidingSphere->mass)) + (OtherNormalVelocity * (CollidingSphere->mass - mass) / (mass + CollidingSphere->mass)) + OtherTangentVelocity;
}

void RTRSphere::PerformCollision(RTRCylinder * CollidingCyllinder)
{
    glm::vec2 NormalisedVelocity = glm::normalize(Velocity);
    

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_Transform, scale, rotation, translation, skew, perspective);
    glm::vec2 MyTwoDTranlsation = glm::vec2(translation.x, translation.z);

    glm::decompose(CollidingCyllinder->m_Transform, scale, rotation, translation, skew, perspective);
    glm::vec2 Other2DTranslation = glm::vec2(translation.x, translation.z);

    glm::vec2 NormalOfCollision = Other2DTranslation - MyTwoDTranlsation;
    NormalOfCollision = glm::normalize(NormalOfCollision);

    if (glm::dot(NormalOfCollision, NormalisedVelocity) / (glm::length(NormalisedVelocity) * glm::length(NormalOfCollision)) < 0)
    {
        return;
    }

    float DistInsideWall = Radius + CollidingCyllinder->Radius - glm::length(Other2DTranslation - MyTwoDTranlsation);

    if (DistInsideWall >= 0.0f)
    {
        float ReflectionAngle = std::acos(glm::dot(NormalOfCollision, NormalisedVelocity) / (glm::length(NormalisedVelocity) * glm::length(NormalOfCollision)));

        float sinTheta = std::cos(ReflectionAngle);
        float DistAlongVelocity = DistInsideWall / sinTheta + 0.001f;

        glm::vec2 TranslationVector = NormalisedVelocity * (-DistAlongVelocity);
        glm::vec3 TranslationVecFinal = glm::vec3(TranslationVector.x, 0.0f, TranslationVector.y);
        m_Transform = glm::translate(m_Transform, TranslationVecFinal);

        //d−2(d⋅n)n

        Velocity = Velocity - 2.0f * (glm::dot(Velocity, NormalOfCollision) * NormalOfCollision);
    }
}

void RTRSphere::PerformCollision(glm::vec2 NormalOfCollision, glm::vec2 IntersectPoint)
{
    glm::vec2 NormalisedVelocity = glm::normalize(Velocity);
    //glm::vec2 NormalOfCollision = glm::vec2(-IntersectedLine.y, IntersectedLine.x);
    //NormalOfCollision = glm::normalize(NormalOfCollision);

    if (glm::dot(NormalOfCollision, NormalisedVelocity) / (glm::length(NormalisedVelocity) * glm::length(NormalOfCollision)) < 0)
    {
        return;
    }

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_Transform, scale, rotation, translation, skew, perspective);

    glm::vec2 TwoDTranlsation = glm::vec2(translation.x, translation.z);

    float DistToWall = glm::length(IntersectPoint - TwoDTranlsation);
    float DistInsideWall = Radius - DistToWall;

    if (DistInsideWall >= 0.0f)
    {
        float ReflectionAngle = std::acos(glm::dot(NormalOfCollision, NormalisedVelocity) / (glm::length(NormalisedVelocity) * glm::length(NormalOfCollision)));

        float sinTheta = std::cos(ReflectionAngle);
        float DistAlongVelocity = DistInsideWall / sinTheta + 0.001f;

        glm::vec2 TranslationVector = NormalisedVelocity * (-DistAlongVelocity);
        glm::vec3 TranslationVecFinal = glm::vec3(TranslationVector.x, 0.0f, TranslationVector.y);
        m_Transform = glm::translate(m_Transform, TranslationVecFinal);

        //d−2(d⋅n)n

        Velocity = Velocity - 2.0f * (glm::dot(Velocity, NormalOfCollision) * NormalOfCollision);
        CollisionCount++;
    }

}

void RTRSphere::PerformCollision(glm::vec2 NormalOfCollision, glm::vec2 IntersectedPoint, RTRFlipper* CollidingFlipper)
{
    PerformCollision(NormalOfCollision, IntersectedPoint);

    float FlipperSpeed = CollidingFlipper->RotationSpeed * 2 * 3.14159f * glm::length(IntersectedPoint - CollidingFlipper->CentreOfRotation) / 20.0f;

    //Velocity perpendicular to the collision
    Velocity -= FlipperSpeed * NormalOfCollision;
    Velocity -= CollidingFlipper->bounciness * NormalOfCollision;
    //glm::vec2 OtherNormalVelocity = FlipperSpeed * -NormalOfCollision;
    //glm::vec2 MyTangentVelocity = Velocity - MyNormalVelocity;

    //Calculate the end velocities
    //Velocity = (MyNormalVelocity * (mass - CollidingFlipper->mass) / (mass + CollidingFlipper->mass)) + (OtherNormalVelocity * (2 * CollidingFlipper->mass) / (mass + CollidingFlipper->mass)) + MyTangentVelocity;
    //CollidingSphere->Velocity = (MyNormalVelocity * (2 * mass) / (mass + CollidingSphere->mass)) + (OtherNormalVelocity * (CollidingSphere->mass - mass) / (mass + CollidingSphere->mass)) + OtherTangentVelocity;
}

bool RTRSphere::SphereRayIntersection(glm::vec2 PointOne, glm::vec2 PointTwo, glm::vec2 & IntersectionPoint, float & OverlapLength)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_Transform, scale, rotation, translation, skew, perspective);

    glm::vec2 RayDirection = PointTwo - PointOne;
    RayDirection = glm::normalize(RayDirection);
    
    float A = std::pow(RayDirection.x, 2.0f) + std::pow(RayDirection.y, 2.0f);
    float B = 2 * (RayDirection.x * (PointOne.x - translation.x) + RayDirection.y * (PointOne.y - translation.z));
    float C = std::pow(PointOne.x - translation.x, 2.0f) + std::pow(PointOne.y - translation.z, 2.0f) - std::pow(Radius,2.0f);

    float d = std::pow(B, 2.0f) - 4 * A * C;

    //First check we have an intersection
    if (d >= 0.0f)
    {
        //Then check if the intersection(s) are within the distance 0 to length of line.
        float t1 = (-B + std::sqrt(d)) / 2 * A;
        float t2 = (-B - std::sqrt(d)) / 2 * A;

        float tMax = glm::length(PointTwo - PointOne);
        bool bT1Valid = (t1 >= 0.0f && t1 <= tMax);
        bool bT2Valid = (t2 >= 0.0f && t2 <= tMax);

        if (t1 < 0.0f && t2 > tMax)
        {
            bT1Valid = bT2Valid = true;
        }
        else if (t2 < 0.0f && t1 > tMax)
        {
            bT1Valid = bT2Valid = true;
        }

        if (bT1Valid || bT2Valid)
        {
            float t3 = (t1 + t2) / 2.0f;
            t3 = glm::clamp(t3, 0.0f, tMax);
            IntersectionPoint = PointOne + t3 * RayDirection;
             
            return true;
        }
    }
    return false;
}

bool RTRSphere::IsColliding(RTRCube* CollidingCube)
{
    return OverlapsInterval(CollidingCube->boundingBoxMin, CollidingCube->boundingBoxMax);
}

bool RTRSphere::IsColliding(RTRCylinder* CollidingCylinder)
{
    float dist = std::abs(CollidingCylinder->CentrePoint.x - CentrePoint.x) + std::abs(CollidingCylinder->CentrePoint.y - CentrePoint.y);
    float distSquared = dist * dist;

    if (distSquared < std::pow(CollidingCylinder->Radius + Radius, 2.0f))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RTRFlipper::CalculateBoundingVolume(glm::mat4 ParentTransform)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(ParentTransform, scale, rotation, translation, skew, perspective);

    CentreOfRotation = glm::vec2(translation.x, translation.z);

    RTRCube::CalculateBoundingVolume(ParentTransform);
}

void RTRCylinder::Init(float radius)
{
    const unsigned int X_SEGMENTS = 32;
    const unsigned int Y_SEGMENTS = 32;
    const float PI = 3.14159265359f;

    m_NumVertices = (X_SEGMENTS + 1) * (Y_SEGMENTS + 1);
    m_NumFaces = X_SEGMENTS * 2 * (Y_SEGMENTS + 1);

    m_Faces = new RTRFace_t[m_NumFaces];

    Radius = radius;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    //Calculate all our points, uvs and normals
    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
    {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI)/* std::sin(ySegment * PI)*/ * radius;
            float yPos = std::cos(ySegment * PI) * radius;
            float zPos = std::sin(xSegment * 2.0f * PI) /* std::sin(ySegment * PI)*/ * radius;
            if (y == 0 || y == Y_SEGMENTS)
            {
                xPos = 0.0f;
                zPos = 0.0f;
            }

            

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    //Calculate the face indices
    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x + 1);

                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x + 1);
                indices.push_back((y) * (X_SEGMENTS + 1) + x + 1);
            }
        }
    }
    indexCount = indices.size();

    for (size_t i = 0; i < indexCount / 3; i++)
    {
        m_Faces[i] = RTRFace_t(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);
    }

    m_Vertices = new RTRPoint_t[m_NumVertices];

    for (unsigned int i = 0; i < positions.size(); ++i)
    {
        RTRPoint_t point;
        point.x = (positions[i].x);
        point.y = (positions[i].y);
        point.z = (positions[i].z);
        if (normals.size() > 0)
        {
            point.normX = (normals[i].x);
            point.normY = (normals[i].y);
            point.normZ = (normals[i].z);
        }
        if (uv.size() > 0)
        {
            point.TexCoordX = (uv[i].x);
            point.TexCoordY = (uv[i].y);
        }

        m_Vertices[i] = point;
    }

    //m_NumVertices = 18;
    //m_NumFaces = 32;
    //m_Vertices = new RTRPoint_t[]
    //{
    //    {       0,  1,       0 },   // Top CCW
    //    {       0,  1,      -1 },
    //    { -0.7071,  1, -0.7071 },
    //    { -1     ,  1,       0 },
    //    { -0.7071,  1,  0.7071 },
    //    {  0,       1,       1 },
    //    { 0.70711,  1,  0.7071 },
    //    {  1,       1,       0 },
    //    {  0.7071,  1, -0.7071 },

    //    {       0, -1,       0 },   // Bottom CCW (viewed from below)
    //    {       0, -1,      -1 },
    //    {  0.7071, -1, -0.7071 },
    //    {  1     , -1,       0 },
    //    {  0.7071, -1,  0.7071 },
    //    {  0,      -1,       1 },
    //    { -0.7071, -1,  0.7071 },
    //    { -1,      -1,       0 },
    //    { -0.7071, -1, -0.7071 }
    //};
    //m_Faces = new RTRFace_t[]
    //{
    //    {  0,  1,  2 }, {  0,  2,  3 },   // Top NW
    //    {  0,  3,  4 }, {  0,  4,  5 },   // SW
    //    {  0,  5,  6 }, {  0,  6,  7 },   // SE 
    //    {  0,  7,  8 }, {  0,  8,  1 },   // NE

    //    {  9, 10, 11 }, {  9, 11, 12 },   // Bottom NW (viewed from below)
    //    {  9, 12, 13 }, {  9, 13, 14 },   // SW
    //    {  9, 14, 15 }, {  9, 15, 16 },   // SE 
    //    {  9, 16, 17 }, {  9, 17, 10 },   // NE

    //    {  1, 10, 17 }, {  1, 17,  2 },   // Wall NW (viewed from above)
    //    {  2, 17, 16 }, {  2, 16,  3 },

    //    {  3, 16, 15 }, {  3, 15,  4 },   // Wall SE (viewed from above)
    //    {  4, 15, 14 }, {  4, 14,  5 },

    //    {  5, 14, 13 }, {  5, 13,  6 },   // Wall SW (viewed from above)
    //    {  6, 13, 12 }, {  6, 12,  7 },

    //    {  7, 12, 11 }, {  7, 11,  8 },   // Wall NE (viewed from above)
    //    {  8, 11, 10 }, {  8, 10,  1 }
    //};
    RTRObject::Init();
}

void RTRCylinder::CalculateBoundingVolume(glm::mat4 ParentTransform)
{
    //Find centre point
    glm::vec4 Origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 BallPosition = m_Transform * Origin;
    CentrePoint = glm::vec2(BallPosition.x, BallPosition.z);
}

bool RTRCylinder::IsColliding(RTRSphere* CollidingSphere)
{
    return CollidingSphere->IsColliding(this);
}

bool RTRCylinder::OverlapsInterval(glm::vec2 MinimumPoint, glm::vec2 MaximumPoint)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_Transform, scale, rotation, translation, skew, perspective);

    float d = 0;

    //Check x axis
    if (translation.x < MinimumPoint.x)
    {
        d += std::pow((translation.x - MinimumPoint.x), 2.0f);
    }
    else if (translation.x > MaximumPoint.x)
    {
        d += std::pow((translation.x - MaximumPoint.x), 2.0f);
    }

    //Check y axis
    if (translation.z < MinimumPoint.y)
    {
        d += std::pow((translation.z - MinimumPoint.y), 2.0f);
    }
    else if (translation.z > MaximumPoint.y)
    {
        d += std::pow((translation.z - MaximumPoint.y), 2.0f);
    }

    //Check if distance from sphere is less than radius
    if (d < std::pow(Radius, 2.0f))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RTRCylinder::CheckForCollision(RTRSphere* CollidingSphere)
{
    CollidingSphere->CheckForCollision(this);
}
