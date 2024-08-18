#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "Rendering/RenderDefinitions.h"

DECLARE( struct, GfxFlags );
DECLARE1( struct, rendering, Gfx );
DECLARE1( struct, rendering, PassResources );
DECLARE1( struct, rendering, VertexBuffer )
DECLARE1( struct, rendering, Material );
DECLARE1( struct, rendering, DeferredFrameBuffer );
DECLARE1( struct, rendering, RenderTarget );
DECLARE1( struct, rendering, Vertex );
DECLARE1( class, rendering, GfxDevice );
DECLARE1( struct, assets, TextureCreationParam );
DECLARE1( class, resources, ResourceContainer );

BEGIN_NAMESPACE2( rendering, fn )

void clear( const GfxFlags& flags );
void setDepth( const GfxFlags& flags );
void setStencil( const GfxFlags& flags );
void setCullFace( const GfxFlags& flags );
void clearFunc( const GfxFlags& flags );

// Shader binding
void bindMaterialTextures( GfxQueue& gfx, GfxDevice* device, ShaderProgram& shader, id::EntityId id );
void bindCameraMatrices( GfxQueue& gfx, ShaderProgram& shader );
void bindLights( GfxQueue& gfx, ShaderProgram& shader );

END_NAMESPACE2

BEGIN_NAMESPACE2( rendering, draw )

void drawSingleLightImpl( GfxQueue& gfx, GfxDevice* device, id::EntityId id, const GfxShader& material );
void drawSingleLitImpl( GfxQueue& gfx, GfxDevice* device, id::EntityId id, const GfxShader& material );
void drawSingleDirectImpl( GfxQueue& gfx, GfxDevice* device, id::EntityId id, const GfxShader& material );

END_NAMESPACE2

BEGIN_NAMESPACE2( rendering, pass )

void executeBindFrameBuffer( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );
void executeClear( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );
void executeBindAndClearFrameBuffer( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );
void executeLit( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );
void executeLights( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );
void executeColorBuffersToFullScreen( GfxQueue& gfx, GfxDevice* device, const PassResources& input );
void executeBlur( GfxQueue& gfx, GfxDevice* device, const PassResources& input );
void executeWriteBorderStencil( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );
void executePixelatedToFullScreen( GfxQueue& gfx, GfxDevice* device, const PassResources& input );
void executeColorCorrectedToFullScreen( GfxQueue& gfx, GfxDevice* device, const PassResources& input );
void executeChromaticAberrationToFullScreen( GfxQueue& gfx, GfxDevice* device, const PassResources& input );
void executeDrawOutlineBorder( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );
void executeDrawSkybox( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );
void executeError( GfxQueue& gfx, GfxDevice* device, const PassResources& resources );

END_NAMESPACE2