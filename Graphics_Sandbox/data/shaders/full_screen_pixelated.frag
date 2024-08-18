#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float fragmentScale;

void main()
{ 
    vec2 uv = TexCoords;
    uv = uv * fragmentScale;
    uv = round(uv);
    uv = uv / fragmentScale;

    outColor = texture(screenTexture, uv);
}
