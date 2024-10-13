#version 450

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec3 Normal;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out vec3 FragPos; 

struct PerObjectData
{
    mat4 model;
};

layout(std140, binding = 4) uniform PerObjectBuffer {
    PerObjectData perObjectBuffer[100];
};

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    mat4 projection;
};

void main() 
{    
    mat4 model = perObjectBuffer[gl_InstanceID].model;
    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = transpose(inverse(mat3(model))) * aNormal;
    TexCoord = aTexCoord;

    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}