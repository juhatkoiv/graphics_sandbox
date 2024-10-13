#version 450

#define TWO_PI_SQUARED 2.50662827463f

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec2 TexCoords;

layout (std140, binding = 4) uniform PostProcessing {
    uint args1;
    uint args2;
    uint args3;
    uint args4;
};

layout (binding = 0) uniform sampler2D screenTexture;

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
    int kernelSize = int(args1);
    float sigma = uintBitsToFloat(args2);
    float windowSizeInvX = uintBitsToFloat(args3);

    int kernel = ensureOdd(kernelSize);
    int halfKernel = (kernel / 2);

    vec4 color = vec4(0.0f);
    float sum = 0.0;
    
    for (int i = -halfKernel; i < halfKernel; ++i)
    {
        float deltaR = windowSizeInvX * i;
        float gauss = gauss1D(deltaR, sigma);
        
        vec2 coordinate = TexCoords.st + vec2(deltaR, 0);
        color += gauss * texture(screenTexture, coordinate);
        sum += gauss;
    }

    outColor = (color / sum);
}