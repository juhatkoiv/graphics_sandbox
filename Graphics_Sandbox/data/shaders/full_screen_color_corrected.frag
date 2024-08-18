#version 460 core

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform vec3 colorCorrection;

void main() 
{  
    vec4 col = texture(screenTexture, TexCoords);
    // Convert to grayscale using luminance calculation
    float luminance = dot(col.xyz, vec3(0.299, 0.587, 0.114));
    
    // TODO _ use gradient texture
    outColor = vec4(colorCorrection * luminance, col.a);
}
