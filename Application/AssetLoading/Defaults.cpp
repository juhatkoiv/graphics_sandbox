#include "Precompiled/Precompiled.h"
#include "Defaults.h"

#include "Rendering/Vertex.h"

BEGIN_NAMESPACE1( assets )

VertexData Meshes::quad()
{
	return VertexData{
		.vertices = {
			{ {  1.f,  1.f, 0.0f }, { 1.f,  1.f, 0.0f }, { 1, 1 } },
			{ { -1.f,  1.f, 0.0f }, { -1.f,  1.f, 0.0f }, { 0, 1 } },
			{ { -1.f, -1.f, 0.0f }, { -1.f, -1.f, 0.0f }, { 0, 0 } },
			{ {  1.f, -1.f, 0.0f }, { 1.f, -1.f, 0.0f }, { 1, 0 } },
		},
		.indices = {
			0, 1, 3,   // first triangle
			1, 2, 3,    // second triangle
		}
	};
}

VertexData Meshes::cube() 
{
    return VertexData {
        .vertices = {
            // Vertex index, position, normal, and UV mapping based on the face definitions
            {{1.f, -1.f, 1.f}, {0.f, -1.f, 0.f}, {1.f, 0.333333f}}, // Vertex 2 - f 2/1/1
            {{-1.f, -1.f, 1.f}, {0.f, -1.f, 0.f}, {0.666667f, 0.666667f}}, // Vertex 3 - f 3/2/1
            {{-1.f, -1.f, -1.f}, {0.f, -1.f, 0.f}, {0.666667f, 0.333333f}}, // Vertex 4 - f 4/3/1
            {{-1.f, 1.f, -1.f}, {0.f, 1.f, 0.f}, {0.333333f, 0.f}}, // Vertex 8 - f 8/1/2
            {{-1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.333333f, 1.f}}, // Vertex 7 - f 7/4/2
            {{1.f, 1.f, 1.f}, {0.f, 1.f, 0.f}, {0.f, 0.666667f}}, // Vertex 6 - f 6/5/2
            {{1.f, 1.f, -1.f}, {1.f, 0.f, 0.f}, {0.666667f, 0.f}}, // Vertex 5 - f 5/6/3
            {{1.f, -1.f, -1.f}, {1.f, 0.f, 0.f}, {0.333333f, 0.f}}, // Vertex 1 - f 1/4/3
            {{1.f, -1.f, 1.f}, {0.f, 0.f, 1.f}, {0.f, 1.f}}, // Vertex 2 - f 2/12/4
            {{-1.f, 1.f, -1.f}, {0.f, 0.f, -1.f}, {0.f, 0.f}}, // Vertex 8 - f 8/11/5
            {{1.f, -1.f, -1.f}, {0.f, 0.f, -1.f}, {1.f, 0.f}} // Vertex 1 - f 1/12/6
        },
        .indices = {
            // Indices - make sure to match these with the above vertex definitions
            0, 1, 2,  // f 2/1/1 3/2/1 4/3/1
            3, 4, 5,  // f 8/1/2 7/4/2 6/5/2
            6, 5, 0,  // f 5/6/3 6/7/3 2/8/3
            0, 5, 1,  // f 2/12/4 6/8/4 3/4/4
            1, 5, 3,  // f 3/9/5 7/10/5 8/11/5
            9, 2, 10, // f 1/12/6 4/13/6 8/11/6
            0, 1, 2,  // f 1/4/1 2/1/1 4/3/1
            6, 3, 5,  // f 5/14/2 8/1/2 6/5/2
            10, 6, 0, // f 1/12/3 5/6/3 2/8/3
            0, 5, 1,  // f 2/12/4 6/8/4 3/4/4
            2, 1, 3,  // f 4/13/5 3/9/5 8/11/5
            6, 10, 3  // f 5/6/6 1/12/6 8/11/6
        }
    };
}

END_NAMESPACE1
