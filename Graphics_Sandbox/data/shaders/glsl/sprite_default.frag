#version 450

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec2 TexCoord;

layout (binding = 0) uniform sampler2D spriteTexture;

void main() 
{   
    vec4 color = texture(spriteTexture, TexCoord);
    if (color.a < 0.1)
        discard;

    outColor = color;
}
