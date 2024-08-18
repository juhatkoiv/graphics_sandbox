#version 460 core

#define LIGHT_COUNT 20

layout (location = 0) out vec4 outColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

struct LightProperties
{
    vec3 lightPos;
    float intensity;
    vec3 lightColor;
    float attenuation;
};

uniform LightProperties lights[LIGHT_COUNT];
uniform vec3 viewPos;
uniform int lightCount;

uniform float diffuseCoeff;
uniform float specularCoeff;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec4 hue;

float calculate_damping_factor(vec3 lightPos, vec3 fragPos, float attenuation)
{
    float d = distance(lightPos, fragPos);
    return pow((1.0 / max(0.1, d)), attenuation);
}

void main() 
{
    vec4 ambientColor = vec4(0.0, 0.0, 0.0, hue.a);
    vec4 diffuseColor = texture(diffuseTexture, TexCoord);
    vec4 specularColor = texture(specularTexture, TexCoord);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 totalDiff = vec3(0);
    vec3 totalSpec = vec3(0);

    for (int i = 0; i < lightCount; i++)
    {
        float damping = calculate_damping_factor(lights[i].lightPos, FragPos, lights[i].attenuation);
        vec3 lightDir = normalize(lights[i].lightPos - FragPos);  
    
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diffuseCoeff * diff * lights[i].lightColor * damping * lights[i].intensity;

        totalDiff += diffuse;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
        vec3 specular = specularCoeff * spec * lights[i].lightColor * damping * lights[i].intensity;

        totalSpec += specular;
    }

    outColor = ambientColor + specularColor * vec4(totalSpec, 1.0) + diffuseColor * vec4(totalDiff, 1.0);  
}
