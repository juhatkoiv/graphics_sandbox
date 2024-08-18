#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float exposure;
uniform float gamma;

void main()
{
    vec4 color =  texture(screenTexture, TexCoords);
    vec3 mapped = pow(vec3(1.0) - exp(-color.rgb * exposure), vec3(1.0 / gamma));

    outColor = vec4(mapped, color.a);
}