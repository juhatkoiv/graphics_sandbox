#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "Resources/Handle.h"
#include "Rendering/Vertex.h"
#include "Rendering/Texture.h"
#include "Result.h"

BEGIN_NAMESPACE1( resources )

class ResourceContainer
{
	const rendering::TextureData EMPTY_TEXTURE{};
	const rendering::VertexData EMPTY_MESH{};
	const rendering::TextureDescriptor EMPTY_TEXTURE_DESCRIPTOR{};

public:
	void addTexture( Handle handle, const rendering::TextureData& param )
	{
		_handleTextureMap.push_back( std::make_tuple( handle, param ) );
	}

	void addTexture (Handle handle, const rendering::TextureDescriptor& param)
	{
		_handleTextureDescriptorMap.push_back( std::make_tuple( handle, param ) );
	}

	void addVertexData( Handle handle, const rendering::VertexData& vertexData )
	{
		_handleVertexDataMap.push_back( std::make_tuple( handle, vertexData ) );
	}

	bool isMesh( Handle handle ) const
	{
		return getVertexData( handle ).success;
	}

	bool isTexture( Handle handle ) const
	{
		return getTextureData( handle ).success;
	}

	bool isTextureDescriptor( Handle handle ) const
	{
		return getTextureDescriptor( handle ).success;
	}
	
	const Result<const rendering::TextureData&> getTextureData( id::TextureId textureId ) const
	{
		resources::Handle textureHandle
		{
			.id = textureId
		};
		return getTextureData( textureHandle );
	}

	const Result<const rendering::TextureData&> getTextureData( Handle handle ) const
	{
		for (auto& [handle_, param] : _handleTextureMap)
		{
			if (handle == handle_)
				return Result<const rendering::TextureData&>::ok( param );;
		}
		return Result<const rendering::TextureData&>::failed();
	}

	const Result<const rendering::VertexData&> getVertexData( id::MeshId meshId ) const
	{
		resources::Handle meshHandle
		{
			.id = meshId
		};
		return getVertexData( meshHandle );
	}

	const Result<const rendering::VertexData&> getVertexData( Handle handle ) const
	{
		for (auto& [handle_, vData] : _handleVertexDataMap)
		{
			if (handle == handle_) 
			{
				return Result<const rendering::VertexData&>::ok( vData );
			}
		}
		return Result<const rendering::VertexData&>::failed();
	}

	const Result<const rendering::TextureDescriptor&> getTextureDescriptor( Handle handle ) const
	{
		for (auto& [handle_, vData] : _handleTextureDescriptorMap)
		{
			if (handle == handle_)
			{
				return Result<const rendering::TextureDescriptor&>::ok( vData );
			}
		}
		return Result<const rendering::TextureDescriptor&>::failed();
	}

	const std::vector<std::tuple<Handle, rendering::TextureData>>& getTextureMap() const 
	{
		return _handleTextureMap;
	}

	const std::vector<std::tuple<Handle, rendering::VertexData>>& getVertexDataMap() const 
	{
		return _handleVertexDataMap;
	}

	const std::vector<std::tuple<Handle, rendering::TextureDescriptor>>& getTextureDescriptorMap() const
	{
		return _handleTextureDescriptorMap;
	}

private:
	std::vector<std::tuple<Handle, rendering::TextureDescriptor>> _handleTextureDescriptorMap;
	std::vector<std::tuple<Handle, rendering::TextureData>> _handleTextureMap;
	std::vector<std::tuple<Handle, rendering::VertexData>> _handleVertexDataMap;
};

END_NAMESPACE1