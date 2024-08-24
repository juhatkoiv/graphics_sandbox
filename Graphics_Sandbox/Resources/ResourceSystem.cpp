#include "Precompiled/Precompiled.h"
#include "ResourceSystem.h"

#include "AppData/AppData.h"
#include "AppData/RepositoryItemTypes.h"
#include "Hash/Hash.h"
#include "AssetLoading/AssimpMeshLoader.h"
#include "AssetLoading/TextureLoader.h"
#include "Rendering/Vertex.h"
#include "Rendering/Texture.h"

BEGIN_NAMESPACE1( resources )

namespace
{

	bool saveNameHandlePair( const std::string& fileName, resources::Handle& handle, std::vector<std::pair<std::string, Handle>>& nameHandleMap )
	{
		bool found = false;

		for (auto& [name, h] : nameHandleMap)
		{
			if (name == fileName)
			{
				found = true;
				handle = h;
				break;
			}
		}

		if (!found)
			nameHandleMap.push_back( std::pair{ fileName, handle } );

		return !found;
	}


	static Result<const std::pair<std::string, Handle>&> getResourceNameHandlePair( resources::Handle handle, const std::vector<std::pair<std::string, Handle>>& nameHandleMap )
	{
		for (const auto& pair : nameHandleMap)
		{
			const auto& h = pair.second;
			if (h != handle)
				continue;

			return Result<const std::pair<std::string, Handle>&>::ok( pair );
		}
		return Result<const std::pair<std::string, Handle>&>::failed();
	}

	static Result<const std::string&> getResourceName( resources::Handle handle, const std::vector<std::pair<std::string, Handle>>& nameHandleMap )
	{
		auto result = getResourceNameHandlePair( handle, nameHandleMap );
		if (!result)
		{
			return Result<const std::string&>::failed();
		}

		return Result<const std::string&>::ok( result.get().first );
	}
}

ResourceSystem::ResourceSystem( const appdata::AppData& config ) noexcept
{
	_textureLoader = assets::TextureLoader();
	_meshLoader = assets::MeshLoader();

	std::span<std::string> meshes = config.getMeshPaths();
	for (const std::string& mesh : meshes)
	{
		addMesh( mesh );
	}

	std::span<std::string> textures = config.getTexturePaths();
	for (const std::string& texture : textures)
	{
		addTexture( texture );
	}

	// TEMP
	id::TextureId textureId = id::NewTextureId();
	rendering::TextureDescriptor descriptor{};
	descriptor.dimensions = rendering::TextureDimensions::TCube;

	std::vector<std::string> skyBoxPaths =
	{
		"data//skyboxes//sky//right.jpg",
		"data//skyboxes//sky//left.jpg",
		"data//skyboxes//sky//top.jpg",
		"data//skyboxes//sky//bottom.jpg",
		"data//skyboxes//sky//front.jpg",
		"data//skyboxes//sky//back.jpg"
	};

	for (const std::string& fileName : skyBoxPaths)
	{
		Result<const rendering::TextureData&> result = _textureLoader.load( fileName, 0 );
		if (!result)
		{
			Logger::LogError( "Failed to load texture: " + fileName );
			assert( result.success );
		}
		descriptor.textureData.push_back( &result.get() );			
	}
	
	_container.addTexture( Handle{ textureId }, descriptor );
}

void ResourceSystem::cleanup() noexcept
{
	_textureLoader.cleanup();
	_meshLoader.cleanup();
}

Handle ResourceSystem::addTexture( const std::string& fileName )
{
	Result<const rendering::TextureData&> result = _textureLoader.load( fileName );
	if (!result)
	{
		Logger::LogError( "Failed to load texture: " + fileName );
		assert( result.success );
	}

	const rendering::TextureData& param = result.get();

	Handle handle{};
	handle.id = id::NewTextureId();

	const bool success = saveNameHandlePair( fileName, handle, _nameHandleMap );
	if (success)
	{
		_container.addTexture( handle, param );
	}

	return handle;
}

Handle ResourceSystem::addMesh( const std::string& fileName )
{
	auto result = _meshLoader.load( fileName );
	if (!result)
	{
		Logger::LogError( "Failed to load mesh: " + fileName );
	}

	const rendering::VertexData& vertexData = result.get();

	Handle handle{};
	handle.id = id::NewMeshId();
	//handle.id = hash::get64(fileName);

	const bool success = saveNameHandlePair( fileName, handle, _nameHandleMap );
	if (success)
	{
		_container.addVertexData( handle, vertexData );
	}

	return handle;
}

Handle ResourceSystem::findHandle( const std::string& fileName )
{
	for (auto& [name, handle_] : _nameHandleMap)
	{
		if (name == fileName)
			return handle_;
	}
	return Handle{};
}

id::TextureId ResourceSystem::findTextureId( const std::string& fileName )
{
	Handle handle = findHandle( fileName );
	if (!_container.isTexture( handle ))
	{
		return id::InvalidTextureId;
	}
	return static_cast<id::TextureId>(handle.id);
}

id::MeshId ResourceSystem::findMeshId( const std::string& fileName )
{
	Handle handle = findHandle( fileName );
	if (!_container.isMesh( handle ))
	{
		return id::InvalidMeshId;
	}
	return static_cast<id::MeshId>(handle.id);
}

id::ShaderId ResourceSystem::findShaderId( const std::string& shaderName )
{
	return shader::getShaderId( shaderName.c_str() );
}

SkyboxArgs ResourceSystem::getSkyboxArgs( const std::string& skyboxName )
{
	id::MeshId meshId = findMeshId( "data//meshes//cube.obj" );

	auto [hadle, desc] = _container.getTextureDescriptorMap().front();

	return SkyboxArgs
	{ 
		.textureId = static_cast<id::TextureId>(hadle.id),
		.meshId = meshId
	};
}


std::string ResourceSystem::getShaderName( id::ShaderId shader ) const
{
	return shader::getShaderName( shader );
}

Result<const std::string&> ResourceSystem::getMeshName( id::MeshId meshId ) const
{
	resources::Handle meshHandle{ meshId };

	const bool isMesh = _container.isMesh( meshHandle );
	if (!isMesh)
	{
		Logger::LogError( "Given mesh id does not exists." );
		assert( isMesh );
	}

	return getResourceName( meshHandle, _nameHandleMap );
}

Result<const std::string&> ResourceSystem::getTextureName( id::TextureId textureId ) const
{
	resources::Handle textureHandle{ textureId };

	const bool isTexture = _container.isTexture( textureHandle );
	if (!isTexture)
	{
		Logger::LogError( "Given texture id does not exists." );
		assert( isTexture );
	}

	return getResourceName( textureHandle, _nameHandleMap );
}

const ResourceContainer& ResourceSystem::getResourceContainer() const
{
	return _container;
}

END_NAMESPACE1