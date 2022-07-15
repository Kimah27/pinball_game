//-----------------------------------------------------------------------------
// RTRCamera.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 DEFAULT_FRONT    = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 DEFAULT_UP       = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 DEFAULT_RIGHT    = glm::vec3(1.0f, 0.0f, 0.0f);
static const float DEFAULT_YAW          = -90.0f;        // degrees
static const float DEFAULT_PITCH        = 0.0f;          // degrees
static const float DEFAULT_MOVE_SPEED   = 4.0f;          // units per second
static const float DEFAULT_STRAFE_SPEED = 4.0f;          // units per second
static const float DEFAULT_TURN_SPEED   = 60.0f;         // degrees per second
static const float DEFAULT_TILT_SPEED   = 60.0f;         // degrees per second

class RTRCamera
{
public:
    RTRCamera(glm::vec3 position = DEFAULT_POSITION, glm::vec3 up = DEFAULT_UP, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH) 
        : m_Position{ position }
        , m_WorldUp{ up }
        , m_Yaw{ yaw }
        , m_Pitch{ pitch }
    { UpdateVectors(); }

    // TODO: Add code to stop camera from flipping
    // TODO: move time multiplication out of this class?? Keep camera simple, and physics separate...
    glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
    void MoveForward(unsigned int td_milli) { m_Position += m_Front * m_MoveSpeed * (td_milli / 1000.0f); }
    void MoveBackward(unsigned int td_milli) { m_Position -= m_Front * m_MoveSpeed * (td_milli / 1000.0f); }
    void StrafeLeft(unsigned int td_milli) { m_Position -= m_Right * m_StrafeSpeed * (td_milli / 1000.0f); }
    void StrafeRight(unsigned int td_milli) { m_Position += m_Right * m_StrafeSpeed * (td_milli / 1000.0f); }
    void TurnLeft(unsigned int td_milli) { m_Yaw -= m_TurnSpeed * td_milli / 1000.0f; UpdateVectors(); }
    void TurnRight(unsigned int td_milli) { m_Yaw += m_TurnSpeed * td_milli / 1000.0f; UpdateVectors(); }
    void TiltUp(unsigned int td_milli) { m_Pitch += m_TiltSpeed * td_milli / 1000.0f; UpdateVectors(); }
    void TiltDown(unsigned int td_milli) { m_Pitch -= m_TiltSpeed * td_milli / 1000.0f; UpdateVectors(); }

    // TODO: make private; add public getters and setters for all of these
    glm::vec3 m_Position;
    glm::vec3 m_WorldUp;
    glm::vec3 m_Front{ DEFAULT_FRONT };
    glm::vec3 m_Up{ DEFAULT_UP };
    glm::vec3 m_Right{ DEFAULT_RIGHT };
    float m_Yaw;
    float m_Pitch;
    float m_MoveSpeed{ DEFAULT_MOVE_SPEED };
    float m_StrafeSpeed{ DEFAULT_STRAFE_SPEED };
    float m_TurnSpeed{ DEFAULT_TURN_SPEED };
    float m_TiltSpeed{ DEFAULT_TILT_SPEED };

private:
    void UpdateVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }
};
