#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos; 

layout (location = 20) uniform mat4 model;
layout (location = 21) uniform mat4 view;
layout (location = 22) uniform mat4 projection;

void main() 
{    
    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = transpose(inverse(mat3(model))) * aNormal;
    TexCoord = aTexCoord;

    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}