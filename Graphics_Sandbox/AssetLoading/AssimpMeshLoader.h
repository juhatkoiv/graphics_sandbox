#pragma once

#include <vector>

#include "Rendering/Vertex.h"

DECLARE1( struct, rendering, VertexData );

BEGIN_NAMESPACE1( assets );

class AssimpMeshLoader
{
public:
	AssimpMeshLoader();
	~AssimpMeshLoader();

	void cleanup();
	Result<const rendering::VertexData&> load( const std::string& fileName );

private:
	std::unordered_map<std::string, rendering::VertexData> _meshCache;
};

using MeshLoader = AssimpMeshLoader;

END_NAMESPACE1