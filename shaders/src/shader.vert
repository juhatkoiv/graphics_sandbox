#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Normal;
out vec2 TexCoord;

void main() 
{
    gl_Position = vec4(aPosition, 1.0);
    Normal = aNormal;
    TexCoord = aTexCoord;
}