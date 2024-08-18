#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoord;

uniform float borderWidth;
uniform vec3 borderColor;
uniform vec2 resolution;

uniform sampler2D spriteTexture;

#define DIV_SQRT_2 0.70710678118

void main() 
{
    vec2 directions[8] = {vec2(1, 0), vec2(0, 1), vec2(-1, 0), vec2(0, -1),
        vec2(DIV_SQRT_2, DIV_SQRT_2), vec2(-DIV_SQRT_2, DIV_SQRT_2),
        vec2(-DIV_SQRT_2, -DIV_SQRT_2), vec2(DIV_SQRT_2, -DIV_SQRT_2)};

    vec2 resolutionInv = 1.0 / resolution;
    vec4 total;
    for (int i = 0; i < 8; i++)
    {
        total += texture(spriteTexture, TexCoord + directions[i] * borderWidth);
    }

    total /= 8.0;
    vec4 color = texture(spriteTexture, TexCoord);
    if (color.a < 0.05)
    {
        if (total.a > 0.05)
        {
            outColor = vec4(borderColor, 1.0f);
        }
    }
    else
        outColor = color;
}