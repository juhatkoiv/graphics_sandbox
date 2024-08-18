#pragma once

#include <glm/glm.hpp>

#include "System/System.h"
#include "Rendering/Texture.h"
#include "ECS/UniqueId.h"
#include "ECS/EntitySpawnParams.h"
#include "Containers/SparseMap.h"
#include "Rendering/RenderEffect.h"
#include "Rendering/RenderingSettings.h"
#include "Resources/ResourceSystem.h"

DECLARE1( struct, rendering, Vertex );
DECLARE1( struct, rendering, RenderingSettings );
DECLARE1( struct, ecs, Light );
DECLARE1( struct, ecs, Mesh );
DECLARE1( class, ecs, World ); 
DECLARE1( class, rendering, RenderCommandQueue );
DECLARE1( class, rendering, GfxDevice );
DECLARE1( class, rendering, GfxWorker );
DECLARE1( class, resources, ResourceContainer );

BEGIN_NAMESPACE1( ecs )

class Renderer : public System
{
public:
	Renderer( rendering::GfxWorker* worker, const resources::ResourceContainer* resourceContainer );
	~Renderer();

	void update( ecs::World& world, float dt );

	void setMaterial( id::EntityId id, const Material& material );
	void setMesh( id::EntityId id, const Mesh& mesh );
	void setFullScreenQuad( const Mesh& mesh );
	void setMeshInstanced( id::EntityId id, const Mesh& mesh );
	void setLight( id::EntityId id, const Light& param );
	void setEffect( id::EntityId id, rendering::RenderEffect effects );
	void setTexture( id::EntityId id, id::TextureId textureId, rendering::TextureType type, const rendering::TextureData& textureData );
	void setSkybox( const Mesh& mesh, id::TextureId textureId );

	void updateModelMatrix( id::EntityId id, const glm::mat4& modelMatrix );
	void updateCameraTransform( const glm::vec3& viewPosition, const glm::mat4& viewMatrix );
	void updateCameraProjection( const glm::mat4& viewMatrix );
	void updateWindow( const glm::uvec2& viewPortSize, const glm::ivec2& viewPortPosition );
	void updateCameraRenderSettings( const rendering::RenderingSettings& settings );

	void destroy( id::EntityId id );

private:
	const resources::ResourceContainer* _resourceContainer = nullptr;
	rendering::GfxWorker* _worker = nullptr;
};

END_NAMESPACE1