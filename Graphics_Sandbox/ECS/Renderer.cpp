#include "Precompiled/Precompiled.h"
#include "Renderer.h"

#include "AssetLoading/TextureLoader.h"
#include "Rendering/Texture.h"
#include "AssetLoading/JsonFileSystem.h"

#include "Resources/ResourceContainer.h"

#include "ECS/Light.h"
#include "ECS/Material.h"
#include "ECS/World.h"
#include "ECS/Camera.h"

#include "Rendering/Vertex.h"
#include "Rendering/RenderCommands.h"
#include "Rendering/RenderCommandQueue.h"
#include "Rendering/GfxDevice.h"
#include "Rendering/GfxWorker.h"

#include <tuple>
#include <memory>
#include <ranges>

BEGIN_NAMESPACE1( ecs )

namespace
{
	struct AABB
	{
		glm::vec3 center;
		glm::vec3 halfSize;

		AABB() = default;
		AABB( glm::vec3 center, glm::vec3 halfSize )
			: center( center ), halfSize( halfSize )
		{}

		bool contains( const AABB& other ) const {
			glm::vec3 distance = glm::abs( center - other.center );
			glm::vec3 totalSize = halfSize + other.halfSize;

			return distance.x <= totalSize.x && distance.y <= totalSize.y && distance.z <= totalSize.z;
		}

		void expand( const AABB& other ) {
			glm::vec3 minCorner = glm::min( center - halfSize, other.center - other.halfSize );
			glm::vec3 maxCorner = glm::max( center + halfSize, other.center + other.halfSize );

			center = (minCorner + maxCorner) * 0.5f;
			halfSize = (maxCorner - minCorner) * 0.5f;
		}
	};
	
	struct Plane {
		glm::vec3 normal;
		float d;

		void normalize()
		{
			float length = glm::length( normal );
			normal /= length;
			d /= length;
		}
	};

	struct Frustum {

		std::array<Plane, 6> planes;
	};

	static Frustum updateCameraFrustum( const Camera* camera )
	{
		glm::mat4 vp = camera->projectionMatrix * camera->viewMatrix;

		Frustum frustum{};

		std::array<Plane, 6>& planes = frustum.planes;
		planes[0].normal = glm::vec3( vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0] ); // Left Plane
		planes[0].d = vp[3][3] + vp[3][0];

		planes[1].normal = glm::vec3( vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0] ); // Right Plane
		planes[1].d = vp[3][3] - vp[3][0];

		planes[2].normal = glm::vec3( vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1] ); // Bottom Plane
		planes[2].d = vp[3][3] + vp[3][1];

		planes[3].normal = glm::vec3( vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1] ); // Top Plane
		planes[3].d = vp[3][3] - vp[3][1];

		planes[4].normal = glm::vec3( vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2] ); // Near Plane
		planes[4].d = vp[3][3] + vp[3][2];

		planes[5].normal = glm::vec3( vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2] ); // Far Plane
		planes[5].d = vp[3][3] - vp[3][2];

		for (auto& plane : planes)
		{
			plane.normalize();
		}
		return frustum;
	}

	static std::tuple<std::vector<id::EntityId>&, std::vector<Transform*>&> getEntitiesWithinFrustum( const Camera* camera,
		const std::span<id::EntityId>& entities, const std::span<Transform*>& transforms )
	{
		Frustum frustum = updateCameraFrustum( camera );

		static std::vector<id::EntityId> entitiesWithinFrustum;
		static std::vector<Transform*> transformsWithinFrustum;

		entitiesWithinFrustum.resize( 0 );
		transformsWithinFrustum.resize( 0 );

		for (size_t i = 0; i < entities.size(); i++)
		{
			glm::vec3 position = transforms[i]->position;
			// TODO - improve heuristics
			float radius = 0.5f * std::max( std::max( transforms[i]->scale.x, transforms[i]->scale.y ), transforms[i]->scale.z );

			bool withinFrustum = true;
			for (const auto& plane : frustum.planes) {
				if (glm::dot( plane.normal, position ) + plane.d < -radius) {
					withinFrustum = false; 
					break; // Sphere is outside of this plane
				}
			}

			if (!withinFrustum)
				continue;

			entitiesWithinFrustum.push_back( entities[i] );
			transformsWithinFrustum.push_back( transforms[i] );
		}
		return std::make_tuple( std::ref( entitiesWithinFrustum ), std::ref( transformsWithinFrustum ) );
	}

	using namespace assets;
	using namespace rendering;

	static inline RenderCommandQueue _renderCommandQueue;

	static void QueueRendererCommand( RenderCommand&& command )
	{
		_renderCommandQueue.queue( std::forward<RenderCommand>( command ) );
	}
}

Renderer::Renderer( rendering::GfxWorker* worker, const resources::ResourceSystem* resourceSystem )
	: _resourceSystem( resourceSystem ), _worker( worker )
{
	auto meshId = _resourceSystem->findMeshId( "data//meshes//quad.obj" );
	ecs::Mesh mesh
	{
		.meshId = meshId
	};
	setFullScreenQuad( mesh );

	auto skybox = _resourceSystem->getSkyboxArgs( "bay" );
	ecs::Mesh skyboxMesh
	{
		.meshId = skybox.meshId
	};
	
	setSkybox( skyboxMesh, skybox.textureId );

	// set ALL resources
}

Renderer::~Renderer() {}

struct ClosestFirstComparator {
	glm::vec3 cameraPos{};
	std::span<Transform*> transforms{};

	ClosestFirstComparator( const glm::vec3& cameraPos, const std::span<Transform*>& transforms )
		: cameraPos( cameraPos ), transforms( transforms ) {}

	bool operator()( size_t a, size_t b ) const
	{
		return glm::distance2( transforms[a]->position, cameraPos ) < glm::distance2( transforms[b]->position, cameraPos );
	}
};

void Renderer::update( ecs::World& world, float dt )
{
	std::span<Camera*> cameras = world.getAll<Camera>();
	for (size_t i = 0; i < cameras.size(); i++)
	{
		auto camera = cameras[i];
		if (camera->viewPortSize.x < 1 || camera->viewPortSize.y < 1)
			return;
	}

	auto [transformEntities, transforms] = world.getPairs<Transform>();
	if (!transforms.empty())
	{
		for (size_t i = 0; i < cameras.size(); i++)
		{
			auto camera = cameras[i];
			if (camera->viewPortSize.x < 1 || camera->viewPortSize.y < 1)
				return;

			auto cameraPos = camera->position;

			updateCameraTransform( camera->position, camera->viewMatrix );
			updateWindow( camera->viewPortSize, camera->viewPortPosition );
			updateCameraProjection( camera->projectionMatrix );
			updateCameraRenderSettings( camera->renderingSettings );

			auto [finalEntities, finalTransforms] = getEntitiesWithinFrustum( camera, transformEntities, transforms );

			std::vector<glm::mat4> modelMatrices;
			Transform::updateModelMatrices( finalTransforms, modelMatrices );

			if (finalEntities.empty())
				break;

			std::vector<size_t> indices( finalEntities.size() );
			std::iota( indices.begin(), indices.end(), 0 );
			std::sort( indices.begin(), indices.end(), ClosestFirstComparator{ cameraPos, finalTransforms } );

			// sort entities by queue
			for (int i = 0; i < indices.size(); i++)
			{
				int index = indices[i];

				id::EntityId entityId = finalEntities[index];

				updateModelMatrix( entityId, modelMatrices[index] );

				if (world.has<Mesh>( entityId )) 
				{
					setMesh( entityId, *world.get<Mesh>( entityId ));
				}
			
				if (world.has<Material>( entityId ))
				{
					setMaterial( entityId, *world.get<Material>( entityId ) );
				}

				if (world.has<RenderEffects>( entityId ))
				{
					auto effects = world.get<RenderEffects>( entityId );
					if (!isSet( effects->effects ))
						continue;

					setEffect( entityId, effects->effects );
				}
			}

			auto [ entities, lights] = world.getPairs<Light>();
			for (int j = 0; j < entities.size(); j++)
			{
				setLight( entities[j], *lights[j] );
			}
		}
	}

	RenderCommand cmd;
	cmd.type = CommandType::PrepareTransparentQueue;
	QueueRendererCommand( std::move( cmd ) );

	_worker->update( _renderCommandQueue );
	
	_worker->render();
}

void Renderer::setMesh( id::EntityId id, const Mesh& mesh )
{
	RenderCommand cmd;
	cmd.type = CommandType::SetMesh;
	cmd.id = id;
	cmd.setEntityMeshCmd = SetEntityMeshCmd
	{
		.meshId = mesh.meshId
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::setFullScreenQuad( const Mesh& mesh )
{
	RenderCommand cmd;
	cmd.type = CommandType::CreateFullScreenQuad;
	cmd.setFullScreeQuadCmd = SetFullScreenQuadCmd
	{
		.meshId = mesh.meshId,
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::setMeshInstanced( id::EntityId id, const Mesh& mesh )
{
	RenderCommand cmd;
	cmd.type = CommandType::SetMeshInstanced;
	cmd.id = id;
	cmd.setEntityMeshCmd = SetEntityMeshCmd
	{
		.meshId = mesh.meshId,
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::setMaterial( id::EntityId id, const Material& material )
{
	RenderCommand cmd;
	cmd.type = CommandType::SetMaterial;
	cmd.id = id;
	cmd.setEntityMaterialCmd = SetEntityMaterialCmd
	{
		.meshColor = material.meshColor,
		.shaderId = material.shaderId,
		.queue = material.renderQueue,
	};

	QueueRendererCommand( std::move( cmd ) );

	auto textureType = material.textureTypes;
	for (size_t i = texture::FIRST_TYPE; i < texture::TYPE_COUNT; i++)
	{
		auto flag = texture::toTypeFlag( i );
		if ((textureType & flag) == TextureType::None)
			continue;

		auto textureId = material.textureIds[i];
		if (textureId == id::InvalidTextureId)
		{
			Logger::LogError( "Attempting to dispatch non-valid texture." );
			continue;
		}

		setTexture( id, textureId, flag );
	}
}

void Renderer::setLight( id::EntityId id, const Light& lightParam )
{
	RenderCommand cmd;
	cmd.id = id;
	cmd.type = CommandType::SetLight;
	cmd.setlightCmd = SetLightCmd
	{ 
		.lightColor = lightParam.lightColor,
		.intensity = lightParam.intensity,
		.attenuation = lightParam.attenuation
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::setEffect( id::EntityId id, RenderEffect effects )
{
	RenderCommand cmd;
	cmd.id = id;
	cmd.type = CommandType::SetEffect;
	cmd.submitToEffectQueueCmd = SubmitToEffectQueueCmd{ effects };

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::setTexture( id::EntityId id, id::TextureId textureId, TextureType type )
{
	RenderCommand cmd;
	cmd.type = CommandType::SetTexture;
	cmd.id = id;
	cmd.setTextureCmd = SetTextureCmd
	{
		.textureId = textureId,
		.type = type
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::setSkybox( const Mesh& mesh, id::TextureId textureId )
{
	RenderCommand cmd;
	cmd.type = CommandType::SetSkyBox;
	cmd.setSkyboxCmd = SetSkyBoxCmd
	{
		.textureId = textureId,
		.meshId = mesh.meshId
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::updateModelMatrix( id::EntityId id, const glm::mat4& modelMatrix )
{
	RenderCommand cmd;
	cmd.type = CommandType::UpdateModelMatrix;
	cmd.id = id;
	cmd.updateModelMatrixCmd = UpdateModelMatrixCmd{ modelMatrix };

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::updateCameraTransform( const glm::vec3& viewPosition, const glm::mat4& viewMatrix )
{
	RenderCommand cmd;
	cmd.type = CommandType::UpdateCameraTransform;
	cmd.updateCameraTrasformCmd = UpdateCameraTransformCmd
	{
		.viewPosition = viewPosition,
		.viewMatrix = viewMatrix
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::updateWindow( const glm::uvec2& viewPortSize, const glm::ivec2& viewPortPosition )
{
	RenderCommand cmd;
	cmd.type = CommandType::UpdateWindowSize;
	cmd.updateWindowSizeCmd = UpdateWindowCmd
	{
		.windowSize = viewPortSize,
		.windowPosition = viewPortPosition
	};
	
	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::updateCameraRenderSettings( const rendering::RenderingSettings& settings )
{
	RenderCommand cmd;
	cmd.type = CommandType::UpdateCameraSettings;
	cmd.updateCameraRenderingSettingsCmd = UpdateCameraRenderSettingsCmd
	{
		.settings = settings
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::updateCameraProjection( const glm::mat4& projectionMatrix )
{
	RenderCommand cmd;
	cmd.type = CommandType::UpdateCameraProjection;
	cmd.updateProjectionMatrixCmd = UpdateProjectionMatrixCmd
	{
		.projectionMatrix = projectionMatrix
	};

	QueueRendererCommand( std::move( cmd ) );
}

void Renderer::destroy( id::EntityId id )
{
	RenderCommand cmd;
	cmd.type = CommandType::Destroy;
	cmd.id = id;
	cmd.destroyEntityCmd = DestroyEntityCmd{};

	QueueRendererCommand( std::move( cmd ) );
}

END_NAMESPACE1