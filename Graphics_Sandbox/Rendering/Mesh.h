#pragma once

#include "VertexBuffer.h"
#include "Vertex.h"

#include <vector>

BEGIN_NAMESPACE1(rendering)

struct Mesh
{
    VertexBuffer vertexBuffer;
    int indexCount;

    GLsizei IndicesCount() 
    {
        return indexCount;
    }
};

END_NAMESPACE1