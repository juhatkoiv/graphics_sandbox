#version 460 core

#define TWO_PI_SQUARED 2.50662827463f

layout (location = 0) out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform int kernelSize;
uniform float sigma;
uniform vec2 windowSizeInverted;

int ensureOdd(int k)
{    
    int result = k;
    if ((result & 0x1) != 0)
    {
        result++;
    }
    return result;
}

float gauss1D(float deltaR, float s)
{    
    return (1.0f / (TWO_PI_SQUARED * s)) * exp(-(pow(deltaR, 2) / (2 * s * s)));
}

void main()
{
    int kernel = ensureOdd(kernelSize);

    int halfKernel = (kernel / 2);

    vec4 color = vec4(0.0f);
    float sum = 0.0;
    
    for (int i = -halfKernel; i < halfKernel; ++i)
    {
        float deltaR = windowSizeInverted.x * i;
        float gauss = gauss1D(deltaR, sigma);
        
        vec2 coordinate = TexCoords.st + vec2(deltaR, 0);
        color += gauss * texture(screenTexture, coordinate);
        sum += gauss;
    }

    outColor = (color / sum);
}