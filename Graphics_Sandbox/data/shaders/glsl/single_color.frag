#version 450

layout (location = 0) out vec4 outColor;


struct Material {
	vec4 hue;
	float diffuseCoeff;
	float specularCoeff;
};

layout(std140, binding = 4) uniform MaterialData {
	Material material[100];
};

layout(std140, binding = 50) uniform PushConstants {
	int modelIndex;
} pushConstants;

void main() 
{
    outColor = material[pushConstants.modelIndex].hue;
}