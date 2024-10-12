#version 460 core

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec3 Normal;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 FragPos; 

void main() 
{
    outColor = vec4(252.0/255.0, 52.0/255.0, 104.0/255.0, 1.0);
}
