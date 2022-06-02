#include "Precompiled.h"
#include "RenderContext.h"

#include "VertexBuffer.h"
#include "Mesh.h"

void RenderContext::CreateVexterBuffer(Mesh& mesh)
{
    VertexBuffer& v = mesh._vertexBuffer;

    const auto& vertices = mesh.Vertices();
    const auto& indices = mesh.Indices(); 

    glGenBuffers(1, &v.vertexObjectBuffer);
    glGenBuffers(1, &v.indexBuffer);

    glGenVertexArrays(1, &v.vertexArrayBuffer);

    glBindVertexArray(v.vertexArrayBuffer);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, v.vertexObjectBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // 4. then set the vertex attributes pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
