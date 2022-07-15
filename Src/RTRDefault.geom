//-----------------------------------------------------------------------------
// RTRDefault.geom
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#version 460
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
    vec3 FragPos;
    vec2 TexCoord;
} gs_in[];

out VertexData {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} gs_out;

void main() {

    vec3 a = vec3(gs_in[1].FragPos - gs_in[0].FragPos);
    vec3 b = vec3(gs_in[2].FragPos - gs_in[0].FragPos);
    vec3 normal = normalize(cross(a, b));

    gl_Position = gl_in[0].gl_Position;
    gs_out.FragPos = gs_in[0].FragPos;
    gs_out.Normal = normal;
    gs_out.TexCoord = gs_in[0].TexCoord;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position;
    gs_out.FragPos = gs_in[1].FragPos;
    gs_out.Normal = normal;
    gs_out.TexCoord = gs_in[1].TexCoord;
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position;
    gs_out.FragPos = gs_in[2].FragPos;
    gs_out.Normal = normal;
    gs_out.TexCoord = gs_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}
