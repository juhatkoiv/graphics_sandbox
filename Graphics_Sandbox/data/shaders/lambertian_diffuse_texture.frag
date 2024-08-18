#version 460 core

#define MAX_LIGHT_COUNT 20

layout (location = 0) out vec4 outColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 viewPos;
uniform int lightCount;

struct LightProperties
{
    vec3 lightPos;
    vec3 lightColor;
    float intensity;
    float attenuation;
};
uniform LightProperties lights[MAX_LIGHT_COUNT];

uniform float diffuseCoeff;
uniform float specularCoeff;

uniform sampler2D diffuseTexture;

float calculate_damping_factor(vec3 lightPos, vec3 fragPos, float attenuation)
{
    float d = distance(lightPos, fragPos);
    return 0.1 + 0.9 * pow((1.0 / max(0.1, d)), attenuation);
}

void main() 
{
    vec4 ambientColor = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 diffuseColor = texture(diffuseTexture, TexCoord);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 totalDiff;
    vec3 totalSpec;

    for (int i = 0; i < lightCount; i++)
    {
        float damping = calculate_damping_factor(lights[i].lightPos, FragPos, lights[i].attenuation);
        vec3 lightDir = normalize(lights[i].lightPos - FragPos);  
    
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diffuseCoeff * diff * lights[i].lightColor * damping * lights[i].intensity;

        totalDiff += diffuse;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
        vec3 specular = specularCoeff * spec * lights[i].lightColor * damping * lights[i].intensity;

        totalSpec += specular;
    }

    outColor = ambientColor + vec4(totalSpec, 1.0) + diffuseColor * vec4(totalDiff, 1.0);  
}