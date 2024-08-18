#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoord;

uniform sampler2D spriteTexture;

void main() 
{   vec4 color = texture(spriteTexture, TexCoord);
    if (color.a < 0.1)
        discard;

    outColor = color;
}
