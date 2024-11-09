#include "Precompiled/Precompiled.h"
#include "AssimpMeshLoader.h"

#include "assimp/Importer.hpp"
#include "assimp/types.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "AssimpVertexParser.h"
#include "Rendering/Vertex.h"
#include "Defaults.h"

#include <sstream>

BEGIN_NAMESPACE1( assets )

namespace
{
	using namespace rendering;

	static VertexData* addToCache( const std::string& fileName, const VertexData& v, std::unordered_map<std::string, VertexData>& cache )
	{
		auto it = cache.find( fileName );
		if (it != cache.end())
		{
			throw;
		}

		cache[fileName] = v;
		return &cache[fileName];
	}

	static Result<const VertexData&> getCached( const std::string& fileName, std::unordered_map<std::string, VertexData>& cache )
	{
		auto it = cache.find( fileName );
		if (it != cache.end())
		{
			auto& v = it->second;
			return Result<const VertexData&>::ok( v );
		}
		return Result<const VertexData&>::failed();
	}

}

AssimpMeshLoader::AssimpMeshLoader()
{
}

AssimpMeshLoader::~AssimpMeshLoader()
{
	cleanup();
}

void AssimpMeshLoader::cleanup()
{
	if (_meshCache.empty())
		return;

	for (auto& it : _meshCache)
	{
		VertexData& vertexData = it.second;
		(void)vertexData;
	}
	_meshCache.clear();
}

Result<const VertexData&> AssimpMeshLoader::load( const std::string& fileName )
{
	if (fileName.empty())
	{
		LOG_ERROR( "Filename empty." );
		assert(!fileName.empty());
	}

	Result<const VertexData&> result = getCached( fileName, _meshCache );
	if (result.success)
	{
		return result;
	}

	Assimp::Importer importer = {};
	const aiScene* scene = importer.ReadFile( fileName, aiProcess_JoinIdenticalVertices );

	aiMesh** assimpMeshes = scene->mMeshes;

	AssimpVertexParser vertexParser;
	VertexData* vertexData = nullptr;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* aiMesh = assimpMeshes[i];
		VertexData parsedData = vertexParser.ParseMesh( aiMesh );

		VertexData* pData = addToCache( fileName, parsedData, _meshCache );
		vertexData = pData;
	}

	if (vertexData == nullptr) 
	{
		return Result<const VertexData&>::failed();
	}

	return Result<const VertexData&>::ok( *vertexData );
}

END_NAMESPACE1