#version 450

layout (location = 0) out vec4 outColor;

layout (binding = 10) uniform Args {
    vec4 fragColor;
};

void main() 
{
    outColor = fragColor;
}