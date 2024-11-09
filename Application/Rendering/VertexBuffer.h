#pragma once

BEGIN_NAMESPACE1(rendering)

struct VertexBuffer 
{
    unsigned int vertexObjectBuffer;
    unsigned int vertexArrayBuffer;
    unsigned int elementArrayObject;
    int indexCount = 0;
};

END_NAMESPACE1