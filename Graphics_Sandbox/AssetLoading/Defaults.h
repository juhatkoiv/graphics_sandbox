#pragma once

#include "Rendering/Vertex.h"

BEGIN_NAMESPACE1(assets)	

struct Meshes 
{
	static rendering::VertexData quad();
	static rendering::VertexData cube();
};

END_NAMESPACE1