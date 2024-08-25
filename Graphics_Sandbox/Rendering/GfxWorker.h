#pragma once

#include "GfxDevice.h"

#include <glm/ext.hpp>

DECLARE1( class, rendering, RenderCommandQueue );
DECLARE1( class, rendering, GfxDeviceFactory );
DECLARE1( class, resources, ResourceContainer );

BEGIN_NAMESPACE1( rendering )

class NonCopyable 
{
public:
	NonCopyable() = default;
	~NonCopyable() = default;
	NonCopyable( const NonCopyable& ) = delete;
	NonCopyable& operator=( const NonCopyable& ) = delete;
};

class GfxWorker : public rendering::NonCopyable
{
private:
	std::unique_ptr<GfxDevice> _device = nullptr;
	rendering::GfxDeviceFactory& _deviceFactory;

public:
	GfxWorker( rendering::GfxDeviceFactory& deviceFactory );
	~GfxWorker() = default;

	void setApi( int api );
	void render();
	void update( RenderCommandQueue& queue );
	void clear();

private:
	void queueDestroy( unsigned id );
	bool isQueuedDestroyed( unsigned id );
	void flushDestroyedQueue();

	//////// UPDATE DATA ////////

	void updateCameraProjectionMatrix( const UpdateProjectionMatrixCmd& param );
	void updateWindow( const UpdateWindowCmd& param );
	void updateCameraTransform( const UpdateCameraTransformCmd& param );
	void setToEffectQueue( unsigned id, const SubmitToEffectQueueCmd& param );
	void setLight( unsigned id, const SetLightCmd& cmd );
	void setMaterial( unsigned id, const SetEntityMaterialCmd& param );
	void setMeshInstanced( unsigned id, const SetEntityMeshCmd& param );
	void setMesh( unsigned id, const SetEntityMeshCmd& param );
	void setTexture( unsigned id, const SetTextureCmd& cmd );
	void createTexture( const CreateTextureCmd& cmd );
	void createMesh( const CreateMeshCmd& cmd );
	void updateModelMatrix( unsigned id, const UpdateModelMatrixCmd& param );
	void updateRenderingSettings( const UpdateCameraRenderSettingsCmd& param );
	void setFullScreenMesh( const SetFullScreenQuadCmd& param );
	void setSkybox( const SetSkyBoxCmd& param );
	void prepareDraw();

	//////// EXECUTION ////////
	
	void executeRenderCommand( const RenderCommand& cmd );
};

END_NAMESPACE1