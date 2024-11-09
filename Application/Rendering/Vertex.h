#pragma once

#include <vector>

BEGIN_NAMESPACE1(rendering)

struct Vertex
{
    float position[3] = { 0.f, 0.f, 0.f };
    float normal[3] = { 0.f, 0.f, 0.f };
    float uv[2] = { 0.f, 0.f };
};

struct VertexIndexed 
{
    float position[3] = { 0.f, 0.f, 0.f };
    float normal[3] = { 0.f, 0.f, 0.f };
    float uv[2] = { 0.f, 0.f };
    unsigned index = 0;
};

struct VertexData
{
    std::vector<Vertex> vertices{};
    std::vector<unsigned> indices{};
};

END_NAMESPACE1