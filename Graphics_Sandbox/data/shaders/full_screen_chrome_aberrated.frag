#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float aberration;

void main()
{     
    vec2 uv = TexCoords;

    float offset = aberration;
    vec2 uvR = uv + vec2(offset, 0);
    vec2 uvG = uv;
    vec2 uvB = uv - vec2(offset, 0);

    vec4 texR = texture(screenTexture, uvR);
    vec4 texG = texture(screenTexture, uvG);
    vec4 texB = texture(screenTexture, uvB);
   
    outColor = vec4(texR.r, texG.g, texB.b, 1.0);       
}
