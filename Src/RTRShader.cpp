//-----------------------------------------------------------------------------
// RTRShader.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include "RTRShader.h"

static char *load_shader_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr) return nullptr;
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    char *buf = new char[size + 1];
    fread(buf, 1, size, fp);
    buf[size] = 0;
    fclose(fp);
    return buf;
}

int RTRShader::Load(const char *vert_path, const char *frag_path, const char *geom_path)
{
    int ret_code = 0;
    char *vert_src = nullptr, *frag_src = nullptr, *geom_src = nullptr;
    unsigned int vert_shader = 0, frag_shader = 0, geom_shader = 0;

    if (vert_path != nullptr && (vert_src = load_shader_file(vert_path)) == nullptr) {
        std::cout << "RTR:ERROR:RTR_Shader:Could not load vertex shader file - " << vert_path << std::endl;
        ret_code = -1;
        goto ERR_RTR_SHADER_LOAD;
    }
    if (frag_path != nullptr && (frag_src = load_shader_file(frag_path)) == nullptr) {
        std::cout << "RTR:ERROR:RTR_Shader:Could not load fragment shader file - " << frag_path << std::endl;
        ret_code = -2;
        goto ERR_RTR_SHADER_LOAD;
    }
    if (geom_path != nullptr && (geom_src = load_shader_file(geom_path)) == nullptr) {
        std::cout << "RTR:ERROR:RTR_Shader:Could not load geometry shader file - " << geom_path << std::endl;
        ret_code = -3;
        goto ERR_RTR_SHADER_LOAD;
    }

    int err_ok;
    char err_info[512];

    if (vert_src != nullptr) {
        vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &vert_src, nullptr);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err_ok);
        if (!err_ok) {
            glGetShaderInfoLog(vert_shader, 512, nullptr, err_info);
            std::cout << "RTR:ERROR: Vertex Shader failed to compile. Error: " << err_info << std::endl;
            ret_code = -4;
            goto ERR_RTR_SHADER_LOAD;
        }
    }
    if (frag_src != nullptr) {
        frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &frag_src, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err_ok);
        if (!err_ok) {
            glGetShaderInfoLog(frag_shader, 512, nullptr, err_info);
            std::cout << "RTR:ERROR: Fragment Shader failed to compile. Error: " << err_info << std::endl;
            ret_code = -5;
            goto ERR_RTR_SHADER_LOAD;
        }
    }
    if (geom_src != nullptr) {
        geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geom_shader, 1, &geom_src, nullptr);
        glCompileShader(geom_shader);
        glGetShaderiv(geom_shader, GL_COMPILE_STATUS, &err_ok);
        if (!err_ok) {
            glGetShaderInfoLog(geom_shader, 512, nullptr, err_info);
            std::cout << "RTR:ERROR: Geometry Shader failed to compile. Error: " << err_info << std::endl;
            ret_code = -6;
            goto ERR_RTR_SHADER_LOAD;
        }
    }

    m_Id = glCreateProgram();
    if (vert_shader != 0) glAttachShader(m_Id, vert_shader);
    if (frag_shader != 0) glAttachShader(m_Id, frag_shader);
    if (geom_shader != 0) glAttachShader(m_Id, geom_shader);
    glLinkProgram(m_Id);
    glGetProgramiv(m_Id, GL_LINK_STATUS, &err_ok);
    if (!err_ok) {
        glGetProgramInfoLog(m_Id, 512, NULL, err_info);
        std::cout << "RTR:ERROR: Shader Program failed to link. Error: " << err_info << std::endl;
        ret_code = -7;
        goto ERR_RTR_SHADER_LOAD;
    }

ERR_RTR_SHADER_LOAD:
    if (vert_shader != 0) glDeleteShader(vert_shader);
    if (frag_shader != 0) glDeleteShader(frag_shader);
    if (geom_shader != 0) glDeleteShader(geom_shader);
    if (vert_src != nullptr) delete vert_src;
    if (frag_src != nullptr) delete frag_src;
    if (geom_src != nullptr) delete geom_src;
    return ret_code;
}
