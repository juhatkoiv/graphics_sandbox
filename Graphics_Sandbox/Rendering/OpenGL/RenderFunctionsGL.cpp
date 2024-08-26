#include "Precompiled/Precompiled.h"
#include "Rendering/RenderFunctions.h"

#include "UniformFunctions.h"

#include "Rendering/GfxDevice.h"
#include "Rendering/RenderDefinitions.h"
#include "Rendering/PassResources.h"

#include <GLFW/glfw3.h>

BEGIN_NAMESPACE2( rendering, fn )

// clearing

void clear( const GfxFlags& flags )
{
	if (flags.clearFlags == ClearFlags::None)
		return;

	int result = 0;
	if (has(flags.clearFlags, ClearFlags::Color))
		result |= GL_COLOR_BUFFER_BIT;

	if (has(flags.clearFlags, ClearFlags::Depth))
		result |= GL_DEPTH_BUFFER_BIT;

	if (has(flags.clearFlags, ClearFlags::Stencil))
		result |= GL_STENCIL_BUFFER_BIT;

	if (result)
	{
		glClear( result );
		if (result & GL_COLOR_BUFFER_BIT)
		{
			auto c = flags.clearColor;
			glClearColor( c.x, c.y, c.z, c.w );
		}
	}
}

void setDepth( const GfxFlags& flags )
{
	const DepthDesc& depth = flags.depthDesc;

	if (has( flags.enableFlags, ClearFlags::Depth ))
	{
		// should write to depthbuffer
		glDepthMask( depth.depthMask );

		glEnable( GL_DEPTH_TEST );

		DepthFunc func = depth.depthFunc;

		if (func == DepthFunc::LEqual)
		{
			glDepthFunc( GL_LEQUAL );
		}

		if (func == DepthFunc::Less)
		{
			glDepthFunc( GL_LESS );
		}

		if (func == DepthFunc::Greater)
		{
			glDepthFunc( GL_GREATER );
		}

		if (func == DepthFunc::GEqual)
		{
			glDepthFunc( GL_GEQUAL );
		}

		if (func == DepthFunc::Always)
		{
			glDepthFunc( GL_ALWAYS );
		}

		if (func == DepthFunc::NotEqual)
		{
			glDepthFunc( GL_NOTEQUAL );
		}

		if (func == DepthFunc::Never)
		{
			glDepthFunc( GL_NEVER );
		}
	}

	if (has( flags.disableFlags, ClearFlags::Depth ))
	{
		glDisable( GL_DEPTH_TEST );
	}

}

void setBlend( const GfxFlags& flags )
{
	if (has( flags.enableFlags, ClearFlags::Blend ))
	{
		const BlendDecs& decs = flags.blendDesc;
		glEnable( GL_BLEND );

		GLuint rpgEquation = static_cast<GLuint>(decs.blendEquation);
		GLuint alphaEquation = static_cast<GLuint>(decs.blendEquationAlpha);
		glBlendEquationSeparate( rpgEquation, alphaEquation );

		GLuint src = static_cast<GLuint>(decs.srcBlend);
		GLuint dst = static_cast<GLuint>(decs.dstBlend);
		GLuint srcAlpha = static_cast<GLuint>(decs.srcBlendAlpha);
		GLuint dstAlpha = static_cast<GLuint>(decs.dstBlendAlpha);
		glBlendFuncSeparate( src, dst, srcAlpha, dstAlpha );

		const auto& c = decs.blendColor;
		glBlendColor( c.r, c.g, c.b, c.a );
	}

	if (has( flags.disableFlags, ClearFlags::Blend ))
	{
		glDisable( GL_BLEND );
	}
}

static void setStencilDescImpl( const StencilDesc& desc )
{
	GLuint stencilMask = desc.sencilMask;
	GLint stencilRef = desc.stencilRef;
	GLenum func = static_cast<GLenum>(desc.stencilFunc);
	glStencilFunc( func, stencilRef, stencilMask );

	GLint stFail = static_cast<GLint>(desc.stencilFailOp);
	GLint dpFail = static_cast<GLint>(desc.stencilDepthFailOp);
	GLint btSucc = static_cast<GLint>(desc.stencilBothOkOp);
	glStencilOp( stFail, dpFail, btSucc );
}

void setStencil( const GfxFlags& flags )
{
	if (has( flags.enableFlags, ClearFlags::Stencil ))
	{
		glEnable( GL_STENCIL_TEST );
		setStencilDescImpl( flags.stencilDesc );
	}

	if (has( flags.disableFlags, ClearFlags::Stencil ))
	{
		glDisable( GL_STENCIL_TEST );
	}
}

static void setCullDescImpl( const CullingDesc& desc )
{
	if (desc.culling == Culling::BackFace)
	{
		glCullFace( GL_BACK );
	}

	if (desc.culling == Culling::FrontFace)
	{
		glCullFace( GL_FRONT );
	}

	if (desc.cullingEquation == CullingEquation::Ccw)
	{
		glFrontFace( GL_CCW );
	}

	if (desc.cullingEquation == CullingEquation::Cw)
	{
		glFrontFace( GL_CW );
	}
}

void setCullFace( const GfxFlags& flags )
{
	if (flags.cullingDesc.culling == Culling::None)
	{
		glDisable( GL_CULL_FACE );
		return;
	}
	else
	{
		glEnable( GL_CULL_FACE );
		setCullDescImpl( flags.cullingDesc );
	}
}

void clearFunc( const GfxFlags& flags )
{
	clear( flags );
	setDepth( flags );
	setStencil( flags );
	setCullFace( flags );
	setBlend( flags );
}

void bindMaterialTextures( GfxQueue& gfx, GfxDevice* device, ShaderProgram& shader, id::EntityId id )
{
	if (gfx.frame->diffuseMaterials.has( id ))
	{
		auto& diffuse = gfx.frame->diffuseMaterials[id];
		shader.setFloat( DIFFUSE_COEFF, diffuse.diffuseCoeff );
		shader.setInt( DIFFUSE_TEXTURE, diffuse.bindPosition );
		auto& texture = gfx.frame->textures[id][texture::index<TextureType::Diffuse>()];
		device->bindTexture( texture, diffuse.bindPosition );
	}

	if (gfx.frame->specularMaterials.has( id ))
	{
		auto& specular = gfx.frame->specularMaterials[id];
		shader.setFloat( SPECULAR_COEFF, specular.specularCoeff );
		shader.setInt( SPECULAR_TEXTURE, specular.bindPosition );
		auto& texture = gfx.frame->textures[id][texture::index<TextureType::Specular>()];
		device->bindTexture( texture, specular.bindPosition );
	}
}

END_NAMESPACE2

BEGIN_NAMESPACE2( rendering, draw )

void drawLitInstancedImpl( GfxQueue& gfx, GfxDevice* device, const GfxShader& material )
{
	auto& shader = device->bindShader( material.shaderId );

	id::MeshId meshId = gfx.frame->instancedData.meshId;

	auto id = *gfx.frame->instancedData.groupEntities.begin();
	auto& entities = gfx.frame->instancedData.groupEntities;

	int instanceId = 0;
	for (auto entity : entities)
	{
		auto& mat = gfx.frame->modelMatrices[entity];
		shader.setMatrix( "perObjectBuffer[" + std::to_string( instanceId++ ) + "].model", mat );
	}
	fn::bindMaterialTextures( gfx, device, shader, id );
	
	device->dispatchIndexedInstancedDirect( meshId, (unsigned)gfx.frame->instancedData.groupEntities.size() );
}

END_NAMESPACE2

BEGIN_NAMESPACE2( rendering, pass )

void executeBindFrameBuffer( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	device->bindRenderTarget( resources.targetFrameBuffer );
}

void executeClear( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	const auto& flags = resources.getFlags();
	fn::clear( flags );
	fn::setDepth( flags );
	fn::setStencil( flags );
	fn::setCullFace( flags );
	fn::setBlend( flags );
}

void executeBindAndClearFrameBuffer( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	device->bindRenderTarget( resources.targetFrameBuffer );

	const auto& flags = resources.getFlags();
	fn::clear( flags );
	fn::setDepth( flags );
	fn::setStencil( flags );
	fn::setCullFace( flags );
}

void executeLit( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	id::ShaderId shaderId = resources.getShaderId();
	if (!gfx.batches.has( shaderId ))
		return;

	const DrawCallBatch& batch = gfx.batches.at( shaderId );

	auto& shader = device->bindShader( shaderId );

	for (auto id : batch.entities)
	{
		auto meshId = gfx.frame->meshIdLookup[id];
		auto& mat = gfx.frame->modelMatrices[id];
		auto& meshColor = gfx.meshColors[id];

		shader.setMatrix( MODEL_MATRIX, mat );
		shader.setVec4( "hue", meshColor );

		fn::bindMaterialTextures( gfx, device, shader, id );
		device->dispatchIndexedDirect( meshId );
	}
}

void executeLights( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	id::ShaderId shaderId = resources.getShaderId();
	if (!gfx.batches.has( shaderId ))
		return;

	const DrawCallBatch& batch = gfx.batches.at( shaderId );

	auto& shader = device->bindShader( shaderId );

	for (auto id : batch.entities)
	{
		auto meshId = gfx.frame->meshIdLookup[id];
		auto& mat = gfx.frame->modelMatrices[id];

		shader.setMatrix( MODEL_MATRIX, mat );
		shader.setVec4( FRAG_COLOR, gfx.meshColors[id] );

		device->dispatchIndexedDirect( meshId );
	}
}

void executePostProcessingEffect( GfxQueue& gfx, GfxDevice* device, const PassResources& input ) 
{
	id::ShaderId shaderId = input.getShaderId();
	device->bindShader( shaderId );

	for (unsigned int i = 0; i < input.sourceFramebuffers.size(); i++)
	{
		const auto sourceBuffer = input.sourceFramebuffers[i];
		device->bindRenderTargetResource( sourceBuffer, i );
	}
	device->dispatchIndexedDirect( gfx.frame->fullScreenQuad );
}

void executeWriteBorderStencil( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	id::ShaderId shaderId = resources.getShaderId();
	if (!gfx.batches.has( shaderId ))
		return;

	// Get entities in effect queue and render


/*	const DrawCallBatch& batch = gfx.batches.at(shaderId);

	for (auto id : batch.entities)
	{
		const auto& material = gfx.shaders[id];
		//draw::drawSingleDirectImpl( gfx, device, id, material );
	}*/
}

void executeDrawSkybox( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	id::ShaderId shaderId = resources.getShaderId();
	auto& shader = device->bindShader( shaderId );

	shader.setMatrix( VIEW_MATRIX, gfx.getViewMatrixStripTransform() );
	shader.setMatrix( PROJECTION_MATRIX, gfx.getProjectionMatrix() );
	shader.setInt( CUBEMAP_TEXTURE, 0 );

	auto vbId = gfx.frame->skyboxMesh;
	auto textureId = gfx.frame->skyboxTexture;

	device->bindTexture( textureId, 0 );
	device->dispatchIndexedDirect( vbId );
}

void executeDrawOutlineBorder( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	id::ShaderId shaderId = resources.getShaderId();
	if (!gfx.batches.has( shaderId ))
		return;

	const DrawCallBatch& batch = gfx.batches.at( shaderId );

	auto& shader = device->bindShader( shaderId );
	
	for (auto id : batch.entities)
	{
		auto meshId = gfx.frame->meshIdLookup[id];
		auto& mat = gfx.frame->modelMatrices[id];

		shader.setMatrix( MODEL_MATRIX, mat );
		shader.setVec4( FRAG_COLOR, glm::vec4{ 1, 0, 0, 1 } );

		device->dispatchIndexedDirect( meshId );
	}
}

void executeError( GfxQueue& gfx, GfxDevice* device, const PassResources& resources )
{
	Logger::LogError( "Error pass executed. Pass misconfigured." );
	assert( false );
}

END_NAMESPACE2