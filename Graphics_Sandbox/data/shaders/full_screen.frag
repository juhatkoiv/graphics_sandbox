#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

layout (binding = 0) uniform sampler2D screenTexture;

layout (std140, binding = 4) uniform PostProcessing {
    uint args1;
    uint args2;
    uint args3;
    uint args4;
};

void main()
{
    float exposure = uintBitsToFloat(args1);
    float gamma = uintBitsToFloat(args2);

    vec4 color =  texture(screenTexture, TexCoords);
    vec3 mapped = pow(vec3(1.0) - exp(-color.rgb * exposure), vec3(1.0 / gamma));

    outColor = clamp(vec4(mapped, color.a), 1.0, 0.0);
}
