#version 460 core

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec3 TexCoords;

layout (binding = 0) uniform samplerCube cubemapTexture;

void main()
{
    outColor = texture(cubemapTexture, TexCoords);
}