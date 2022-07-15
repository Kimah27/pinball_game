//-----------------------------------------------------------------------------
// RTRDefault.vert
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#version 460
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform float u_CurTime;
uniform float u_TimeDelta;

out VertexData {
    vec3 FragPos;
    vec2 TexCoord;
} vs_out;

vec3 lerp(vec3 v0, vec3 v1, float t) {
    return (1.0f-t)*v0 + t*v1;
}

void main() {
    vs_out.FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_out.FragPos, 1.0);

    vs_out.TexCoord = a_TexCoord;
}
