#version 450

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec3 Normal;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 FragPos;

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

void main() {
    outColor = vec4(1.0, 1.0, 1.0, 1.0);
}