#include "Precompiled/Precompiled.h"
#include "AssimpVertexParser.h"

#include "assimp/types.h"
#include "assimp/scene.h"

BEGIN_NAMESPACE1(assets);

namespace
{
	using namespace rendering;

	static constexpr int INDICES_PER_FACE = 3;

	static void ParseVertices(const aiMesh* mesh, std::vector<rendering::Vertex>& vertices) 
	{
		vertices.resize(mesh->mNumVertices);

		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			rendering::Vertex& vertex = vertices[i];

			aiVector3D& aiVertex = mesh->mVertices[i];
			aiVector3D& aiNormal = mesh->mNormals[i];
			aiVector3D& aiTexCoord = mesh->mTextureCoords[0][i];

			vertex.position[0] = aiVertex.x;
			vertex.position[1] = aiVertex.y;
			vertex.position[2] = aiVertex.z;

			vertex.normal[0] = aiNormal.x;
			vertex.normal[1] = aiNormal.y;
			vertex.normal[2] = aiNormal.z;
			
			vertex.uv[0] = aiTexCoord.x;
			vertex.uv[1] = aiTexCoord.y;
		}
	}

	static void ParseIndices(const aiMesh* mesh, std::vector<unsigned int>& indices)
	{
		auto indexCount = static_cast<int>(mesh->mNumFaces * INDICES_PER_FACE);
		indices.resize(indexCount);
		
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				int index = (int)(i * face.mNumIndices + j);
				indices[index] = face.mIndices[j];
			}
		}
	}
}

VertexData AssimpVertexParser::ParseMesh(const aiMesh* mesh)
{
	if (mesh == nullptr) 
	{
		LOG_ERROR("Mesh nullptr");
		throw;
	}
	
	VertexData data;
	ParseVertices(mesh, data.vertices);
	ParseIndices(mesh, data.indices);

	return data;
}

END_NAMESPACE1