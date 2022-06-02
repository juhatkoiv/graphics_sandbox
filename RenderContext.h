#pragma once

struct Mesh;

class RenderContext
{
public:
    RenderContext() = default;

    void CreateVexterBuffer(Mesh& mesh);
};

