#version 460 core

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec2 TexCoords;

layout (binding = 0)  uniform sampler2D screenTexture;

void main()
{ 
    outColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}
