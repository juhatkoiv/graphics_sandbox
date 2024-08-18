#pragma once

#include "Rendering/GfxDevice.h"
#include "Precompiled/PrecompilationMacros.h"
#include "Rendering/RenderDefinitions.h"

DECLARE1( class, resources, ResourceContainer );
DECLARE1( struct, window, WindowSize );

BEGIN_NAMESPACE1( rendering )


class OpenGLShaderStorage
{
private:
	struct ShaderMetaData 
	{
		std::vector<std::string> bindingNames{};
		std::string shaderName{};
	};
	LinearMap<id::ShaderId, ShaderProgram, 10> _shadersLookup{};
	LinearMap<id::ShaderId, ShaderMetaData> _shaderMetaData;
	std::string loadShaderSource( const std::string& path ) { return {}; }

public:
	OpenGLShaderStorage() 
	{
		
	}

	ShaderProgram& getShader( id::ShaderId shaderId ) 
	{
		assert( _shadersLookup.has( shaderId ) );
		return _shadersLookup[shaderId];
	}

	const std::vector<std::string>& getBindingNames( id::ShaderId shaderId ) const 
	{
		assert( _shaderMetaData.has( shaderId ) );
		return _shaderMetaData.at(shaderId).bindingNames;
	}
};

class GfxDeviceOpenGL : public GfxDevice
{
private:
	LinearMap<RenderTargetType, RenderTarget, (size_t)RenderTargetType::COUNT> _renderTargetLookup{};
	LinearMap<id::ShaderId, ShaderProgram, 15> _shadersLookup{};
	LinearMap<id::TextureId, GfxTexture, 10> _textureLookup{};
	LinearMap<id::MeshId, VertexBuffer, 10> _vertexBufferLookup{};
	LinearMap<id::ConsBufferId, VSConstBuffer, 10> _vsConstBuffers;
	LinearMap<id::ConsBufferId, PSConstBuffer, 10> _psConstBuffers;
	
	id::ShaderId _activeShader = id::InvalidShaderId;

	// TODO -
	OpenGLShaderStorage _shaderStorage;

	void bindShaderArgsImpl( id::ShaderId shaderId, ShaderProgram& shader, const GLShaderArgs& args );
	bool shaderBound() const { return _activeShader != id::InvalidShaderId; }
public:
	GfxDeviceOpenGL( const GfxDeviceArgs& args );
	~GfxDeviceOpenGL() = default;

	GfxTexture createTexture( id::TextureId id, unsigned bindPosition, Dimensions dimensions, const TextureData& textureData ) override;
	GfxTexture createTexture( id::TextureId textureId, unsigned bindPosition, const TextureDescriptor& descriptor ) override;
	VertexBuffer createVertexBuffer( id::MeshId meshId, const rendering::VertexData& vertexData ) override;
	RenderTarget createRenderTarget( unsigned int width, unsigned int height ) override;

	ShaderProgram& bindShader( id::ShaderId id ) override;
	ShaderProgram& bindShader( id::ShaderId shaderId, const ShaderArgs& args ) override;
	GfxTexture& bindTexture( id::TextureId textureId, unsigned bindPosition = ~0u ) override;
	RenderTarget& bindRenderTarget( RenderTargetType type ) override;
	void bindRenderTargetResource( RenderTargetType type, unsigned bindPosition ) override;
	
	void clear() override;

	// DRAW 

	void dispatchIndexedDirect( id::MeshId mesh ) override;
	void dispatchIndexedInstancedDirect( id::MeshId meshId, unsigned instanceCount ) override;

	// VIEWPORT
	void setViewport( int posX, int posY, int width, int height ) override;

	// RENDER TARGETS

	void deleteRenderTarget( RenderTarget& frameBuffer ) override;
	void recreateRenderTarget( RenderTargetType type, unsigned int width, unsigned int height ) override;
	void recreateRenderTarget( RenderTarget& buffer, unsigned int width, unsigned int height ) override;

	// DELETE
	void deleteVexterBuffer( const VertexBuffer& v ) override;
	void deleteVextexBuffer( id::MeshId meshId ) override;

};

END_NAMESPACE1
