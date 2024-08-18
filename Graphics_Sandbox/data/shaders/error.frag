#version 460 core

layout (location = 0) out vec4 outColor;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos; 

void main() 
{
    outColor = vec4(252.0/255.0, 52.0/255.0, 104.0/255.0, 1.0);
}
