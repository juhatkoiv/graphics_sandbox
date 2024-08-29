#pragma once

#include <memory>

#include "Precompiled/PrecompilationMacros.h"
#include "RenderDefinitions.h"

DECLARE1( class, rendering, RenderCommandQueue );
DECLARE1( struct, rendering, Gfx );
DECLARE1( class, window, Window );
DECLARE1( struct, rendering, TextureData );
DECLARE1( struct, assets, VertexData );

BEGIN_NAMESPACE1( rendering )

struct ApiShaderArgs
{
	virtual ~ApiShaderArgs() = default;
};

template<typename T>
concept ShaderArgsConcept = requires(T t)
{
	{ t.apiArgs() } -> std::convertible_to<ApiShaderArgs*>;
};

enum class ArgType 
{
	Int32,
	Float,
	Vec2,
	Vec3,
	Vec4,
	Mat4,
	Texture2D,
	ConstBuffer,
};

union ArgValue
{
	int intValue;
	float floatValue;
	glm::vec2 vec2Value;
	glm::vec3 vec3Value;
	glm::vec4 vec4Value;
	glm::mat4 mat4Value;
	id::TextureId textureId;
	uint8_t* constBuffer;
};

struct MetaData 
{
	size_t size; 
	unsigned bindPosition;
	unsigned bufferUsage;
};

struct GLShaderArgs : public ApiShaderArgs
{
	static inline constexpr size_t MAX_ARG_COUNT = 10;
	
	std::array<ArgType, MAX_ARG_COUNT> argTypes;
	std::array<ArgValue, MAX_ARG_COUNT> argValues;
	std::array<MetaData, MAX_ARG_COUNT> metaData;
	size_t argCount = 0;

	void validate() const
	{
		assert( argTypes.size() == argValues.size() );
	}

	size_t count() const 
	{
		return argTypes.size();
	}
};

struct ShaderArgs
{
	template<typename T>
	ShaderArgs( T t )
	{
		_data = memory::create( t, sizeof( T ) );
	}

	~ShaderArgs()
	{
		memory::deallocate( _data );
	}

	template<typename T>
	const T& get() const
	{
		return *reinterpret_cast<const T*>(_data);
	}

	const uint8_t* _data = nullptr;
};

struct GfxDeviceArgs
{
	glm::vec2 windowSize{};
	std::vector<id::ShaderId> shaderIds{};
	std::vector<const char*> vertexShaderFiles{};
	std::vector<const char*> fragmentShaderFiles{};

	std::vector<const rendering::VertexData*> vertexData{};
	std::vector<id::MeshId> meshIds{};
	std::vector<const TextureData*> textureData{};
	std::vector<id::TextureId> textureIds{};
	std::vector<TextureDescriptor> textureDescriptors{};
	std::vector<id::TextureId> textureDescriptorIds{};
};

struct GfxBuffer 
{
	void* data = nullptr;
	int size = 0; 
	int usage = 0;
};

// THE API FOR CONSTANT BUFFERS SHOULD BE
/* 

auto handle = device->AllocateConstBuffer(size, usage, bindPosition);

if (handle.dirty())
{
	device->UpdateConstBuffer( handle, data, size );
	// and
	device->UpdateDynamicBuffer( handle, data, size );
}


*/


class GfxDevice
{
public:
	GfxDevice() = default;
	virtual ~GfxDevice() = default;
	
	virtual GfxTexture createTexture( id::TextureId textureId, unsigned bindPosition, Dimensions dimensions, const TextureData& textureData ) = 0;
	virtual GfxTexture createTexture( id::TextureId textureId, unsigned bindPosition, const TextureDescriptor& descriptor ) = 0;
	virtual VertexBuffer createVertexBuffer( id::MeshId meshId, const rendering::VertexData& vertexData ) = 0;
	virtual RenderTarget createRenderTarget( unsigned int width, unsigned int height ) = 0;
	virtual GfxHandle allocateConstantBuffer( int size, int usage, int bindPosition ) = 0;

	virtual ShaderProgram& bindShader( id::ShaderId id ) = 0;
	virtual ShaderProgram& bindShader( id::ShaderId id, const ShaderArgs& args ) = 0;
	virtual GfxTexture& bindTexture( id::TextureId textureId, unsigned bindPosition = ~0u ) = 0;
	virtual RenderTarget& bindRenderTarget( RenderTargetType type ) = 0;
	virtual void bindRenderTargetResource( RenderTargetType type, unsigned bindPosition ) = 0;
	virtual void updateConstantBuffer( GfxHandle buffer, void* data, int size, int offset ) = 0;
	

	// drawing 
	virtual void dispatchIndexedDirect( id::MeshId meshId ) = 0;
	virtual void dispatchIndexedInstancedDirect( id::MeshId meshId, unsigned instanceCount ) = 0;

	virtual void clear() = 0;

	virtual void setViewport( int posX, int posY, int width, int height ) = 0;

	virtual void recreateRenderTarget( RenderTarget& buffer, unsigned int width, unsigned int height ) = 0;
	virtual void recreateRenderTarget( RenderTargetType type, unsigned int width, unsigned int height ) = 0;
	virtual void deleteRenderTarget( RenderTarget& buffer ) = 0;
	virtual void deleteVextexBuffer( id::MeshId meshId ) = 0;
	virtual void deleteVexterBuffer( const VertexBuffer& v ) = 0;

	/*virtual DeferredFrameBuffer createDeferredFrameBuffer( unsigned int width, unsigned int height ) = 0;
	//void renderFrame( const Frame& frame );*/
};

END_NAMESPACE1