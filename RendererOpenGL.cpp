#include "Precompiled.h"

#include "RendererOpenGL.h"
#include "ShaderProgram.h"
#include "RenderContext.h"
#include "Mesh.h"

#include <vector>
#include "Camera.h"

namespace
{
    struct Quad : public Mesh 
    {
        static const std::vector<Vertex> &GetVertices()
        {
            static std::vector<Vertex> vertices =
            {
                { { -0.5f, -0.5f, 0.0f }, { -0.5f, -0.5f, 0.0f }, { 0, 0 } },
                { {  0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f }, { 1, 0 } },
                { {  0.5f,  0.5f, 0.0f }, { 0.5f,  0.5f, 0.0f }, { 1, 1 } },
                { { -0.5f,  0.5f, 0.0f }, { -0.5f,  0.5f, 0.0f }, { 0, 1 } }
            };
            return vertices;
        }

        static const std::vector<unsigned int>& GetIndices()
        {
            static std::vector<unsigned int> indices =
            {
                // note that we start from 0!
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
            };
            return indices;
        }


        const std::vector<Vertex>& Vertices() const override
        {
            return GetVertices();
        };

        const std::vector<unsigned int>& Indices() const override
        {
            return GetIndices();
        };

    };

    static constexpr const char* SHADER_BASIC = "SHADER_BASIC";

    Quad o1;
    Quad o2;
    std::unique_ptr<Camera> camera;

    RenderContext context;

}

RendererOpenGL::RendererOpenGL()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        LOG_ERROR_GL(glewGetErrorString(err));
    }

    LOG_INFO_GL("Status: Using GLEW ", glewGetString(GLEW_VERSION));
    LOG_INFO_GL("OpenGL shader language version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

    _shadersImpl[SHADER_BASIC].reset(new ShaderProgram("shaders/src/shader.vert", "shaders/src/shader.frag"));

    context.CreateVexterBuffer(o1);
    context.CreateVexterBuffer(o2);

    camera.reset(new Camera(glm::vec3(0, 0, -3)));
}

void RendererOpenGL::Render()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);

    _shadersImpl[SHADER_BASIC]->Bind();
    
    auto &indices = Quad::GetIndices();
    glBindVertexArray(o1._vertexBuffer.vertexArrayBuffer);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindVertexArray(o2._vertexBuffer.vertexArrayBuffer);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
