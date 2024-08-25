#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

layout (binding = 0) uniform sampler2D screenTexture;
layout (location = 10) uniform float exposure;
layout (location = 11) uniform float gamma;

void main()
{
    vec4 color =  texture(screenTexture, TexCoords);
    vec3 mapped = pow(vec3(1.0) - exp(-color.rgb * exposure), vec3(1.0 / gamma));

    outColor = clamp(vec4(mapped, color.a), 1.0, 0.0);
}
