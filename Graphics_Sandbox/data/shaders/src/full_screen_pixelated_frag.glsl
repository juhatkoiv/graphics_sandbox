#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float fragmentScale;

void main()
{ 
    vec2 uv = TexCoords.st;
    uv = uv * fragmentScale;
    uv = round(uv);

    outColor = vec4(texture(screenTexture, uv).xyz, 1.0);
}
