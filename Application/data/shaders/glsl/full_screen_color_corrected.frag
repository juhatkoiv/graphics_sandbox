#version 450

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
    vec4 col = texture(screenTexture, TexCoords);
    // Convert to grayscale using luminance calculation
    const float luminance = dot(col.xyz, vec3(0.299, 0.587, 0.114));
    
    float r = uintBitsToFloat(args1);
    float g = uintBitsToFloat(args2);
    float b = uintBitsToFloat(args3);
    
    // TODO _ use gradient texture for mapping
    outColor = vec4(vec3(r,g,b) * luminance, col.a);
}
