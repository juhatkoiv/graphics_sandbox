#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	FragPos = vec3(model * vec4(aPosition, 1.0));
	TexCoord = aTexCoord;

	mat4 modelView = view * model;
	modelView[0][0] = 1;
	modelView[0][1] = 0;
	modelView[0][2] = 0;
	modelView[1][0] = 0;
	modelView[1][1] = 1;
	modelView[1][2] = 0;
	modelView[2][0] = 0;
	modelView[2][1] = 0;
	modelView[2][2] = 1;
	
	gl_Position = projection * modelView * vec4(aPosition, 1.0);
}