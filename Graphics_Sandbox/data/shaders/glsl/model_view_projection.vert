#version 450

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec3 Normal;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out vec3 FragPos; 

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

layout(std140, binding = 0) uniform CameraData {
	mat4 view;
	mat4 viewNoTransform;
	mat4 projection;
} camera;

layout(location = 20) uniform mat4 model;

void main() {
	FragPos = vec3(model * vec4(aPosition, 1.0));
	Normal = transpose(inverse(mat3(model))) * aNormal;
	TexCoord = aTexCoord;
	gl_Position = camera.projection * camera.view * model * vec4(aPosition, 1.0);
}