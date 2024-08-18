#version 460 core

layout (location = 0) out vec4 outColor;

in vec3 TexCoords;

uniform samplerCube cubemapTexture;

void main()
{
    outColor = texture(cubemapTexture, TexCoords);
}