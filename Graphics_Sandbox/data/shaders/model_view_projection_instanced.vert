#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos; 

struct PerObjectBuffer
{
    mat4 model;
};
uniform PerObjectBuffer perObjectBuffer[100];

uniform mat4 view;
uniform mat4 projection;

void main() 
{    
    mat4 model = perObjectBuffer[gl_InstanceID].model;
    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = transpose(inverse(mat3(model))) * aNormal;
    TexCoord = aTexCoord;

    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}