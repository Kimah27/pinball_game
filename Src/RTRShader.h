//-----------------------------------------------------------------------------
// RTRShader.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "RTRLighting.h"
#include "RTRCamera.h"

#define MAX_UNIFORM_NAME 128

class RTRShader
{
public:
    RTRShader() {}
    ~RTRShader() { if (m_Id != 0) glDeleteProgram(m_Id); }
    int Load(const char* vert_path, const char* frag_path, const char* geom_path = nullptr);
    unsigned int GetId() const { return m_Id; }

private:
    unsigned int m_Id{ 0 };

public:
    void SetBool(const char *name, bool value) const { glUniform1i(glGetUniformLocation(m_Id, name), (int)value); }
    void SetInt(const char *name, int value) const { glUniform1i(glGetUniformLocation(m_Id, name), value); }
    void SetFloat(const char *name, float value) const { glUniform1f(glGetUniformLocation(m_Id, name), value); }
    void SetVec2(const char *name, const glm::vec2 &value) const { glUniform2fv(glGetUniformLocation(m_Id, name), 1, &value[0]); }
    void SetVec2(const char *name, float x, float y) const { glUniform2f(glGetUniformLocation(m_Id, name), x, y); }
    void SetVec3(const char *name, const glm::vec3 &value) const { glUniform3fv(glGetUniformLocation(m_Id, name), 1, &value[0]); }
    void SetVec3(const char *name, float x, float y, float z) const { glUniform3f(glGetUniformLocation(m_Id, name), x, y, z); }
    void SetVec4(const char *name, const glm::vec4 &value) const { glUniform4fv(glGetUniformLocation(m_Id, name), 1, &value[0]); }
    void SetVec4(const char *name, float x, float y, float z, float w) const { glUniform4f(glGetUniformLocation(m_Id, name), x, y, z, w); }
    void SetMat2(const char *name, const glm::mat2 &mat) const { glUniformMatrix2fv(glGetUniformLocation(m_Id, name), 1, GL_FALSE, &mat[0][0]); }
    void SetMat3(const char *name, const glm::mat3 &mat) const { glUniformMatrix3fv(glGetUniformLocation(m_Id, name), 1, GL_FALSE, &mat[0][0]); }
    void SetMat4(const char *name, const glm::mat4 &mat) const { glUniformMatrix4fv(glGetUniformLocation(m_Id, name), 1, GL_FALSE, &mat[0][0]); }

    void SetArrayBool(const char* array_name, int array_index, bool value) { SetBool(MakeArrayName(array_name, array_index), value); }
    void SetArrayInt(const char* array_name, int array_index, int value) { SetInt(MakeArrayName(array_name, array_index), value); }
    void SetArrayFloat(const char* array_name, int array_index, float value) { SetFloat(MakeArrayName(array_name, array_index), value); }
    void SetArrayVec2(const char* array_name, int array_index, const glm::vec2& value) { SetVec2(MakeArrayName(array_name, array_index), value); }
    void SetArrayVec2(const char* array_name, int array_index, float x, float y) { SetVec2(MakeArrayName(array_name, array_index), x, y); }
    void SetArrayVec3(const char* array_name, int array_index, const glm::vec3& value) { SetVec3(MakeArrayName(array_name, array_index), value); }
    void SetArrayVec3(const char* array_name, int array_index, float x, float y, float z) { SetVec3(MakeArrayName(array_name, array_index), x, y, z); }
    void SetArrayVec4(const char* array_name, int array_index, const glm::vec4& value) { SetVec4(MakeArrayName(array_name, array_index), value); }
    void SetArrayVec4(const char* array_name, int array_index, float x, float y, float z, float w) { SetVec4(MakeArrayName(array_name, array_index), x, y, z, w); }
    void SetArrayMat2(const char* array_name, int array_index, const glm::mat2& mat) { SetMat2(MakeArrayName(array_name, array_index), mat); }
    void SetArrayMat3(const char* array_name, int array_index, const glm::mat3& mat) { SetMat3(MakeArrayName(array_name, array_index), mat); }
    void SetArrayMat4(const char* array_name, int array_index, const glm::mat4& mat) { SetMat4(MakeArrayName(array_name, array_index), mat); }

    void SetArrayStructBool(const char* array_name, int array_index, const char* element_name, bool value) { SetBool(MakeArrayStructName(array_name, array_index, element_name), value); }
    void SetArrayStructInt(const char* array_name, int array_index, const char* element_name, int value) { SetInt(MakeArrayStructName(array_name, array_index, element_name), value); }
    void SetArrayStructFloat(const char* array_name, int array_index, const char* element_name, float value) { SetFloat(MakeArrayStructName(array_name, array_index, element_name), value); }
    void SetArrayStructVec2(const char* array_name, int array_index, const char* element_name, const glm::vec2& value) { SetVec2(MakeArrayStructName(array_name, array_index, element_name), value); }
    void SetArrayStructVec2(const char* array_name, int array_index, const char* element_name, float x, float y) { SetVec2(MakeArrayStructName(array_name, array_index, element_name), x, y); }
    void SetArrayStructVec3(const char* array_name, int array_index, const char* element_name, const glm::vec3& value) { SetVec3(MakeArrayStructName(array_name, array_index, element_name), value); }
    void SetArrayStructVec3(const char* array_name, int array_index, const char* element_name, float x, float y, float z) { SetVec3(MakeArrayStructName(array_name, array_index, element_name), x, y, z); }
    void SetArrayStructVec4(const char* array_name, int array_index, const char* element_name, const glm::vec4& value) { SetVec4(MakeArrayStructName(array_name, array_index, element_name), value); }
    void SetArrayStructVec4(const char* array_name, int array_index, const char* element_name, float x, float y, float z, float w) { SetVec4(MakeArrayStructName(array_name, array_index, element_name), x, y, z, w); }
    void SetArrayStructMat2(const char* array_name, int array_index, const char* element_name, const glm::mat2& mat) { SetMat2(MakeArrayStructName(array_name, array_index, element_name), mat); }
    void SetArrayStructMat3(const char* array_name, int array_index, const char* element_name, const glm::mat3& mat) { SetMat3(MakeArrayStructName(array_name, array_index, element_name), mat); }
    void SetArrayStructMat4(const char* array_name, int array_index, const char* element_name, const glm::mat4& mat) { SetMat4(MakeArrayStructName(array_name, array_index, element_name), mat); }

    void SetMaterial(const char* name, RTRMaterial_t& mat) {
        SetVec3(MakeStructName(name, "Ambient"), mat.Ambient);
        SetVec3(MakeStructName(name, "Diffuse"), mat.Diffuse);
        SetVec3(MakeStructName(name, "Specular"), mat.Specular);
        SetFloat(MakeStructName(name, "Shininess"), mat.Shininess);
    }
    void SetArrayMaterial(const char* name, int array_index, RTRMaterial_t& mat) {
        char full_name[MAX_UNIFORM_NAME+1];
        sprintf_s(full_name, MAX_UNIFORM_NAME, "%s[%d].", name, array_index);
        SetVec3(MakeStructName(full_name, "Ambient"), mat.Ambient);
        SetVec3(MakeStructName(full_name, "Diffuse"), mat.Diffuse);
        SetVec3(MakeStructName(full_name, "Specular"), mat.Specular);
        SetFloat(MakeStructName(full_name, "Shininess"), mat.Shininess);
    }

    void SetLight(const char* name, RTRLight_t& light) {
        SetInt(MakeStructName(name, "Type"), (int)light.Type);
        SetVec3(MakeStructName(name, "Ambient"), light.Ambient);
        SetVec3(MakeStructName(name, "Diffuse"), light.Diffuse);
        SetVec3(MakeStructName(name, "Specular"), light.Specular);
        SetVec3(MakeStructName(name, "Direction"), light.Direction);
        SetVec3(MakeStructName(name, "Position"), light.Position);
        SetFloat(MakeStructName(name, "Constant"), light.Constant);
        SetFloat(MakeStructName(name, "Linear"), light.Linear);
        SetFloat(MakeStructName(name, "Quadratic"), light.Quadratic);
        SetFloat(MakeStructName(name, "CutOff"), light.Cutoff);
        SetFloat(MakeStructName(name, "OuterCutoff"), light.OuterCutoff);
    }
    void SetArrayLight(const char* name, int array_index, RTRLight_t& light) {
        char full_name[MAX_UNIFORM_NAME + 1];
        sprintf_s(full_name, MAX_UNIFORM_NAME, "%s[%d]", name, array_index);
        SetInt(MakeStructName(full_name, "Type"), (int)light.Type);
        SetVec3(MakeStructName(full_name, "Ambient"), light.Ambient);
        SetVec3(MakeStructName(full_name, "Diffuse"), light.Diffuse);
        SetVec3(MakeStructName(full_name, "Specular"), light.Specular);
        SetVec3(MakeStructName(full_name, "Direction"), light.Direction);
        SetVec3(MakeStructName(full_name, "Position"), light.Position);
        SetFloat(MakeStructName(full_name, "Constant"), light.Constant);
        SetFloat(MakeStructName(full_name, "Linear"), light.Linear);
        SetFloat(MakeStructName(full_name, "Quadratic"), light.Quadratic);
        SetFloat(MakeStructName(full_name, "CutOff"), light.Cutoff);
        SetFloat(MakeStructName(full_name, "OuterCutoff"), light.OuterCutoff);
    }
    void SetLightingModel(RTRLightingModel& light_model) {
        SetInt("u_NumLights", light_model.GetNumLights());
        for (int i = 0; i < light_model.GetNumLights(); i++) {
            RTRLight_t* light = light_model.GetLight(i);
            SetArrayLight("u_Lights", i, *light);
        }
    }

    void SetCamera(const char* name, RTRCamera& camera) {
        SetVec3(MakeStructName(name, "Position"), camera.m_Position);
        SetVec3(MakeStructName(name, "Front"), camera.m_Front);
        SetVec3(MakeStructName(name, "Up"), camera.m_Up);
        SetVec3(MakeStructName(name, "Right"), camera.m_Right);
    }

private:
    char m_UniformName[MAX_UNIFORM_NAME + 1]{ 0 };
    char* MakeArrayName(const char* array_name, int array_index) {
        sprintf_s(m_UniformName, MAX_UNIFORM_NAME, "%s[%d]", array_name, array_index);
        return m_UniformName;
    }
    char *MakeArrayStructName(const char* array_name, int array_index, const char* element_name) {
        sprintf_s(m_UniformName, MAX_UNIFORM_NAME, "%s[%d].%s", array_name, array_index, element_name);
        return m_UniformName;
    }
    char* MakeStructName(const char* struct_name, const char* element_name) {
        sprintf_s(m_UniformName, MAX_UNIFORM_NAME, "%s.%s", struct_name, element_name);
        return m_UniformName;
    }
};
