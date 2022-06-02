#version 460 core

layout (location = 0) out vec4 outColor;

in vec3 Normal;
in vec2 TexCoord;

void main() 
{
    outColor = vec4(1.0, 0.5, 0.5, 1.0);
}