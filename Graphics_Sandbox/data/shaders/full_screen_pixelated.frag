#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

layout (binding = 0)  uniform sampler2D screenTexture;
layout (location = 10) uniform float fragmentScale;

void main()
{ 
    vec2 uv = TexCoords;
    uv = uv * fragmentScale;
    uv = round(uv);
    uv = uv / fragmentScale;

    outColor = texture(screenTexture, uv);
}
