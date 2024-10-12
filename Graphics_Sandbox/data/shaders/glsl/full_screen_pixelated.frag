#version 460 core

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec2 TexCoords;

layout (binding = 0)  uniform sampler2D screenTexture;

layout (std140, binding = 4) uniform PostProcessing {
    uint args1;
    uint args2;
    uint args3;
    uint args4;
};

void main()
{ 
    vec2 uv = TexCoords;
    float fragmentScale = uintBitsToFloat(args1);

    uv = uv * fragmentScale;
    uv = round(uv);
    uv = uv / fragmentScale;

    outColor = texture(screenTexture, uv);
}
