#version 450

/*
#ifdef VULKAN
	// Vulkan-specific code
	layout(push_constant) uniform PushConstants {
		int modelIndex;
	} pushConstants;
	#define MODEL_INDEX pushConstants.modelIndex
#else
	// OpenGL-specific code
	layout(location = 99) uniform int modelIndex;
	#define MODEL_INDEX modelIndex
#endif
*/

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec3 Normal;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out vec3 FragPos; 

layout(std140, binding = 0) uniform CameraData {
	mat4 view;
	mat4 viewNoTransform;
	mat4 projection;
} camera;

layout(std140, binding = 20) uniform ModelData {
    mat4 modelMatrixes[100];
} modelData;

layout(std140, binding = 50) uniform PushConstants {
	int modelIndex;
} pushConstants;

void main() {
	mat4 model = modelData.modelMatrixes[pushConstants.modelIndex];
	
	FragPos = vec3(model * vec4(aPosition, 1.0));
	Normal = transpose(inverse(mat3(model))) * aNormal;
	TexCoord = aTexCoord;
	
	gl_Position = camera.projection * camera.view * model * vec4(aPosition, 1.0);
}