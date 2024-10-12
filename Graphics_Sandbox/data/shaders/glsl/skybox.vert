#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    mat4 projection;
};

layout (location = 0) out vec3 TexCoords;

void main() 
{    
    TexCoords = aPosition;
    vec4 pos = projection * view * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}