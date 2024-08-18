#pragma once
#include <vector>

#include "Rendering/Vertex.h"

struct aiMesh;

BEGIN_NAMESPACE1(assets);

class AssimpVertexParser
{
public:
	rendering::VertexData ParseMesh(const aiMesh *mesh);
};

END_NAMESPACE1