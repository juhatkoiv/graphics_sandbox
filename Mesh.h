#pragma once

#include "VertexBuffer.h"

#include <vector>

struct Vertex
{
	float position[3] = { 0.f, 0.f, 0.f };
    float normal[3] = { 0.f, 0.f, 0.f };
    float uv[2] = { 0.f, 0.f };
};

struct Mesh
{
    VertexBuffer _vertexBuffer;
    
    virtual const std::vector<Vertex>& Vertices() const = 0;
    virtual const std::vector<unsigned int>& Indices() const = 0;
};
