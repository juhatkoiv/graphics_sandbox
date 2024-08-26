#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

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

    float offset = uintBitsToFloat(args1);
    vec2 uvR = uv + vec2(offset, 0);
    vec2 uvG = uv;
    vec2 uvB = uv - vec2(offset, 0);

    vec4 texR = texture(screenTexture, uvR);
    vec4 texG = texture(screenTexture, uvG);
    vec4 texB = texture(screenTexture, uvB);
   
    outColor = vec4(texR.r, texG.g, texB.b, 1.0);       
}
