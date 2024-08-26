#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos; 

layout (location = 20) uniform mat4 model;

layout(std140, binding = 0) uniform CameraData {
    mat4 view;
    mat4 projection;
};

void main() 
{
    mat4 upscale = mat4(1.1);
    upscale[3].w = 1.0;

    mat4 scaledModel = model * upscale;

    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = transpose(inverse(mat3(model))) * aNormal;
    TexCoord = aTexCoord;

    gl_Position = projection * view * scaledModel * vec4(aPosition, 1.0);

}