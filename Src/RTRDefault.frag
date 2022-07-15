//-----------------------------------------------------------------------------
// RTRDefault.frag
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#version 460
out vec4 f_FragColor;

in VertexData {
    vec3 FragPos;
    vec3 Normal; 
    vec2 TexCoord;
} fs_in;

struct RTRCamera {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
};

#define RTR_MAX_LIGHTS 10

#define RTRDirectionalLight    0
#define RTRPointLight          1
#define RTRSpotLight           2

struct RTRLight {
    int Type;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Direction;
    vec3 Position;
    float Constant;
    float Linear;
    float Quadratic;
    float Cutoff;
    float OuterCutoff;
};

struct RTRMaterial {
    vec3 Ambient;
    vec3 Diffuse;
    sampler2D TexDiffuse;
    vec3 Specular;
    sampler2D TexSpecular;
    float Shininess;
};

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

uniform int         u_NumLights;
uniform RTRLight    u_Lights[RTR_MAX_LIGHTS];
uniform RTRMaterial u_ObjectMaterial;
uniform RTRCamera   u_Camera;

uniform float u_CurTime;

uniform sampler2D ourTexture;
uniform sampler2D ourSpecular;

uniform bool isTextured;

void main() 
{
    //vec3 N = normalize(cross(dFdx(fs_in.FragPos), dFdy(fs_in.FragPos)));
    vec3 N = normalize(transpose(inverse(mat3(u_ModelMatrix))) * fs_in.Normal);
    
    vec3 final_color = vec3(0.0, 0.0, 0.0);
    vec3 final_tex_color = vec3(0.0, 0.0, 0.0);

    for (int cur_light=0; cur_light<u_NumLights; cur_light++) {
        float attenuation = 1.0;

        // calc ambient
        vec3 ambient = u_Lights[cur_light].Ambient * u_ObjectMaterial.Ambient;
    
        // calc diffuse
        vec3 L;
        if (u_Lights[cur_light].Type == RTRDirectionalLight) {
            L = -u_Lights[cur_light].Direction;
        }
        else {
            L = u_Lights[cur_light].Position - fs_in.FragPos;
            float Ld = length(L);
            attenuation = min(
                1.0 / (
                  (u_Lights[cur_light].Constant) +
                  (u_Lights[cur_light].Linear * Ld) +
                  (u_Lights[cur_light].Quadratic * Ld * Ld)
                ), 1);
        }
        L = normalize(L);
        float d = max(dot(N, L), 0.0);
        
        vec3 diffuse = u_Lights[cur_light].Diffuse * u_ObjectMaterial.Diffuse * d;
        vec3 texDiffuse = u_Lights[cur_light].Diffuse * d * texture(u_ObjectMaterial.TexDiffuse, fs_in.TexCoord).rgb;
    
        // calc specular
        vec3 V = normalize(u_Camera.Position - fs_in.FragPos);
        // Phong
        //vec3 R = reflect(-L, N);  
        //float s = pow(max(dot(R, V), 0.0), u_ObjectMaterial.Shininess);
        // Blinn-Phong
        vec3 H = normalize(L + V);
        float s = pow(max(dot(N, H), 0.0), u_ObjectMaterial.Shininess);

        vec3 specular = u_Lights[cur_light].Specular * u_ObjectMaterial.Specular * s;
        vec3 texSpecular = u_Lights[cur_light].Specular * s * texture(u_ObjectMaterial.TexSpecular, fs_in.TexCoord).rgb;

        final_color += (ambient + attenuation*(diffuse + specular));
        final_tex_color += (ambient + attenuation*(texDiffuse + specular));
        //final_tex_color += (ambient + attenuation*(texDiffuse + texSpecular));
    }
    
    if (isTextured)
    {
        f_FragColor = (texture(ourTexture, fs_in.TexCoord) * vec4(final_tex_color, 1.0f)) * (texture(ourSpecular, fs_in.TexCoord) * vec4(final_tex_color, 1.0f));
        //f_FragColor = texture(ourTexture, fs_in.TexCoord) * vec4(final_tex_color, 1.0f);
    }
    else
    {
        f_FragColor = vec4(final_color, 1.0);
    }
}

