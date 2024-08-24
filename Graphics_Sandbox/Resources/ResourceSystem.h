#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "Handle.h"
#include "ResourceContainer.h"

#include "AssetLoading/TextureLoader.h"
#include "AssetLoading/AssimpMeshLoader.h"

#include <string>
#include <vector>

DECLARE1( class, appdata, AppData );

BEGIN_NAMESPACE1( resources )

struct SkyboxArgs 
{
	id::TextureId textureId;
	id::MeshId meshId;
};

class ResourceSystem
{
private:
	assets::TextureLoader _textureLoader;
	assets::AssimpMeshLoader _meshLoader;

	std::vector<std::pair<std::string, Handle>> _nameHandleMap;
	ResourceContainer _container;

public:
	ResourceSystem( const appdata::AppData& config ) noexcept;

	void cleanup() noexcept;
	Handle addTexture( const std::string& fileName );
	Handle addMesh( const std::string& fileName );
	
	Handle findHandle( const std::string& fileName ) const;
	
	id::TextureId findTextureId( const std::string& fileName ) const;
	id::MeshId findMeshId( const std::string& fileName ) const;
	id::ShaderId findShaderId( const std::string& shaderName ) const;
	SkyboxArgs getSkyboxArgs( const std::string& skyboxName ) const;
	std::string getShaderName( id::ShaderId shader ) const;
	Result<const std::string&> getMeshName( id::MeshId mesh ) const;
	Result<const std::string&> getTextureName( id::TextureId texture ) const;

	const ResourceContainer& getResourceContainer() const;
};

END_NAMESPACE1
