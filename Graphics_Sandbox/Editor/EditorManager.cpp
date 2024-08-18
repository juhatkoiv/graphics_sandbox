#include "Precompiled/Precompiled.h"
#include "EditorManager.h"

#include <algorithm>
#include <random>
#include <fstream>
#include <filesystem>
#include <ranges>

#include "AppData/ConfigRepository.h"
#include "AssetLoading/JsonFileSystem.h"

#include "ECS/ComponentIncludes.h"
#include "ECS/EntityMoveParam.h"
#include "ECS/ModelSerializer.h"
#include "ECS/World.h"

#include "Rendering/RenderingSettings.h"
#include "Input/InputState.h"

#include "Scene/SceneData.h"
#include "Resources/ResourceSystem.h"

BEGIN_NAMESPACE1( editor )

using namespace ecs;
using namespace scene;

namespace
{
	struct IntersectionResult
	{
		id::EntityId foundEntity = id::InvalidEntityId;
		bool success = false;

		static IntersectionResult getFailed()
		{
			return IntersectionResult
			{
				.foundEntity = ~0u,
				.success = false
			};
		}
	};

	IntersectionResult findFirstIntersectedEntity( const world::Ray& ray, World& world )
	{
		const float stepSize = 0.05f;
		const float rayDistance = 100.f;

		float travelledDistance = 0.0f;

		auto entities = world.getEntitiesWith<Transform>();

		// TODO - This is no good. Replace with spatial partitioning and line intersection check.
		while (travelledDistance < rayDistance)
		{
			glm::vec3 currentRayPos = ray.GetRayEnd( travelledDistance );
			for (auto entityId : entities)
			{
				const auto* transorm = world.get<Transform>( entityId );
				float radius = std::min( transorm->scale.x, std::min( transorm->scale.y, transorm->scale.z ) );
				if (glm::distance2( currentRayPos, transorm->position ) < radius * radius)
				{
					return IntersectionResult
					{
						.foundEntity = entityId,
						.success = true
					};
				}
			}

			travelledDistance += stepSize;
		}
		return IntersectionResult::getFailed();
	}
}

EditorManager::EditorManager( ecs::World& world, resources::ResourceSystem& resourceSystem, const appdata::ConfigRepository& configRepository )
	: _world( world ), _resourceSystem( resourceSystem ), _configReposiotry( configRepository )
{}

void EditorManager::syncWithScene( scene::SceneData& scene )
{
	auto sceneEntities = scene.sceneEntities;
	for (int i = 0; i < sceneEntities.size(); i++)
	{
		auto id = sceneEntities[i];

		if (_editorTransformLookup.has( id ))
			continue;

		if (!_world.has<Transform>( id ))
			continue;

		Transform& transform = *_world.getMutable<Transform>( id );
		updateEditorTransform( id, transform.position, transform.scale );
	}
}

id::EntityId EditorManager::AddVoxelEntityToScene( EntitySpawnParam spawnParams, scene::SceneData& scene )
{
	ModelSerializer modelSerializer( _world, _resourceSystem );
	auto componentBuffers = modelSerializer.deserialize( "data//models//test_model.json" );

	auto entity = _world.spawn( componentBuffers );
	_world.set( entity,
		ecs::Transform
		{
			.position = spawnParams.position,
			.scale = spawnParams.scale,
			.rotation = glm::angleAxis( spawnParams.deltaAngle, spawnParams.rotationAxis )
		} );

	scene.sceneEntities.push_back( entity );
	updateEditorTransform( entity, spawnParams.position, spawnParams.scale );

	return entity;
}

id::EntityId EditorManager::AddLightToScene( ecs::EntitySpawnParam spawnParams, scene::SceneData& scene )
{
	ModelSerializer modelSerializer( _world, _resourceSystem );
	auto componentBuffers = modelSerializer.deserialize( "data//models//test_light_model.json" );

	auto entity = _world.spawn( componentBuffers );
	_world.set( entity,
		ecs::Transform
		{
			.position = spawnParams.position,
			.scale = spawnParams.scale,
			.rotation = glm::angleAxis( spawnParams.deltaAngle, spawnParams.rotationAxis )
		} );
	
	scene.sceneEntities.push_back( entity );
	updateEditorTransform( entity, spawnParams.position, spawnParams.scale );
	return entity;
}

id::EntityId EditorManager::addTransparentEntityToScene( ecs::EntitySpawnParam spawnParams, scene::SceneData& scene )
{
	ModelSerializer modelSerializer( _world, _resourceSystem );
	auto componentBuffers = modelSerializer.deserialize( "data//models//test_model_transparent.json" );

	auto entity = _world.spawn( componentBuffers );
	_world.set( entity,
		ecs::Transform
		{
			.position = spawnParams.position,
			.scale = spawnParams.scale,
			.rotation = glm::angleAxis( spawnParams.deltaAngle, spawnParams.rotationAxis )
		} );

	scene.sceneEntities.push_back( entity );
	updateEditorTransform( entity, spawnParams.position, spawnParams.scale );
	return entity;

}

EditorTransform& EditorManager::GetEditorTransform( id::EntityId id )
{
	assert( _editorTransformLookup.has( id ) );
	return _editorTransformLookup[id];
}

Light& EditorManager::GetLightProperties( id::EntityId id )
{
	return *_world.getMutable<Light>( id );
}

Material& EditorManager::GetMaterialProperties( id::EntityId id )
{
	return *_world.getMutable<Material>( id );
}

Mesh& EditorManager::GetMeshProperties( id::EntityId id )
{
	return *_world.getMutable<Mesh>( id );
}

void EditorManager::deleteSelectedEntity()
{
	if (_selectedEntity == id::InvalidEntityId)
		return;

	_world.destroyEntity( _selectedEntity );
	_selectedEntity = id::InvalidEntityId;

	LOG_INFO( "Deleted selected entity" );
}

Result<const std::string&> EditorManager::getMeshName( id::MeshId meshId )
{
	return _resourceSystem.getMeshName( meshId );
}

Result<const std::string&> EditorManager::getTextureName( id::TextureId textureId )
{
	return _resourceSystem.getTextureName( textureId );
}

void EditorManager::generateRandomizedVoxelTestScene( scene::SceneData& scene )
{
	const int objectCount = 100;
	std::mt19937 rng( (int)0 );
	for (int i = 0; i < objectCount; i++)
	{
		std::uniform_int_distribution<int> genX( -20, 20 ); // uniform, unbiased
		std::uniform_int_distribution<int> genY( -20, 20 ); // uniform, unbiased
		std::uniform_int_distribution<int> genZ( -20, 20 ); // uniform, unbiased
		AddVoxelEntityToScene( { { genX( rng ), genY( rng ), genZ( rng )}, { 1,1,1 }, glm::vec3( 1.0f ), glm::vec3( 1.0f ), glm::vec4( 1.0f ), 0.0f }, scene );
	}
}

void EditorManager::generateTerrain( scene::SceneData& scene )
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			std::uniform_int_distribution<int> genY( -1, 1 ); // uniform, unbiased
			AddVoxelEntityToScene( { { 2.0f * i, -5.0f, 2.0f * j }, { 1,1,1 }, glm::vec3( 1.0f ), glm::vec3( 1.0f ), glm::vec4( 1.0f ), 0.0f }, scene );
		}
	}
}

void EditorManager::generateRandomizedLights( scene::SceneData& scene )
{
	std::mt19937 rng( (int)1 );
	int j = 0;
	for (int i = 0; i < 10; i++, j++)
	{
		std::uniform_int_distribution<int> genX( -20, 20 ); // uniform, unbiased
		std::uniform_int_distribution<int> genY( -20, 20 ); // uniform, unbiased
		std::uniform_int_distribution<int> genZ( -20, 20 ); // uniform, unbiased

		const float r = 1.0f;
		const float g = 1.0f;
		const float b = 1.0f;

		ecs::EntitySpawnParam param
		{
			.position = { genX( rng ), genY( rng ), genZ( rng ) },
				.scale = { 0.2f,0.2f,0.2f },
				.rotationAxis = { 1,1,1 },
				.lightColor = { r,g,b },
				.meshColor = { r, g, b, 1 },
				.deltaAngle = 0.0,
		};
		AddLightToScene( param, scene );
	}
}

const std::vector<RenderResourcesJson>& EditorManager::getRenderResources() const {
	return _configReposiotry.getRenderResources();
}

const RenderGraphJson& EditorManager::getRenderGraph() const {
	return _configReposiotry.getRenderGraph();
}

void EditorManager::clearEditorScene()
{
	_selectedEntity = id::InvalidEntityId;
	_editorTransformLookup.clear();
}

void EditorManager::updateEditorTransform( id::EntityId id, const glm::vec3& position, const glm::vec3& scale )
{
	EditorTransform et;
	et.position = position;
	et.scale = scale;
	et.angleScalars = glm::vec3{ 0, 0, 0 };

	_editorTransformLookup[id] = et;
}

id::MeshId EditorManager::findMeshId( const std::string& meshId )
{
	return _resourceSystem.findMeshId( meshId );
}

void EditorManager::updateEditorTransform( id::EntityId id, const EditorTransform& newTransform, EntityMoveParam& moveParam )
{
	assert( _editorTransformLookup.has( id ) );

	EditorTransform* editorTransform = &_editorTransformLookup[id];

	moveParam.entityId = id;
	moveParam.deltaPosition = newTransform.position - editorTransform->position;
	moveParam.deltaScale = newTransform.scale - editorTransform->scale;

	auto angleScalars = newTransform.angleScalars - editorTransform->angleScalars;
	if (abs( angleScalars.x ) >= FLT_EPSILON)
	{
		moveParam.rotationAxis = global::X_AXIS;
		moveParam.deltaAngle = angleScalars.x;
	}

	if (abs( angleScalars.y ) >= FLT_EPSILON)
	{
		moveParam.rotationAxis = global::Y_AXIS;
		moveParam.deltaAngle = angleScalars.y;
	}

	if (abs( angleScalars.z ) >= FLT_EPSILON)
	{
		moveParam.rotationAxis = global::Z_AXIS;
		moveParam.deltaAngle = angleScalars.z;
	}

	_editorTransformLookup[id] = newTransform;
}

void EditorManager::update()
{
	world::Ray ray{};
	if (input::popRay( ray ))
	{
		auto foundEntities = findFirstIntersectedEntity( ray, _world );
		if (foundEntities.success)
		{
			auto selectedEntity = foundEntities.foundEntity;
			setSelected( selectedEntity );
		}
	}
}

void EditorManager::moveEntity( const EntityMoveParam& moveParam )
{
	Transform* transform = _world.getMutable<Transform>( moveParam.entityId );
	transform->moveBy( moveParam.deltaPosition );
	transform->scaleBy( moveParam.deltaScale );
	if (abs( moveParam.deltaAngle ) >= FLT_EPSILON && glm::length( moveParam.rotationAxis ) >= FLT_EPSILON)
	{
		transform->rotateBy( moveParam.rotationAxis, moveParam.deltaAngle );
	}
}

void EditorManager::setMaterial( id::EntityId id, const Material& material )
{
	_world.set( id, material );
}

void EditorManager::setLight( id::EntityId id, const Light& light )
{
	_world.set( id, light );
}

void EditorManager::setMesh( id::EntityId id, const Mesh& mesh )
{
	_world.set( id, mesh );
}

void EditorManager::setEffect( id::EntityId id, rendering::RenderEffect effect )
{
	if (_world.has<ecs::RenderEffects>( id ))
	{
		ecs::RenderEffects* component = _world.getMutable<ecs::RenderEffects>( id );
		if (effect == rendering::RenderEffect::None)
			component->effects = rendering::RenderEffect::None;
		else
			component->effects = effect;
	}
	else
	{
		_world.set<ecs::RenderEffects>( id, ecs::RenderEffects
		{
			.effects = effect
		} );
	}
}

void EditorManager::setFullScreenEffect( rendering::PostProcessingEffects effect )
{
	auto cameras = _world.getAllMutable<ecs::Camera>();
	for (int i = 0; i < cameras.size(); i++)
	{
		auto* camera = cameras[i];
		camera->renderingSettings.fullScreenEffect = effect;
	}
}

void EditorManager::setSelected( id::EntityId id )
{
	if (_selectedEntity != id::InvalidEntityId)
		setEffect( _selectedEntity, rendering::RenderEffect::None );

	_selectedEntity = id;
	setEffect( _selectedEntity, rendering::RenderEffect::HighlightOutline3D );
}

void EditorManager::setBlurSettings( const rendering::FullScreenBlurSettings& blurSettings )
{
	auto cameraEntities = _world.getEntitiesWith<ecs::Camera>();
	for (int i = 0; i < cameraEntities.size(); i++)
	{
		auto* camera = _world.getMutable<ecs::Camera>( cameraEntities[i] );
		camera->renderingSettings.fullScreenBlurSettings = blurSettings;
	}
}

void EditorManager::setPixelationSettings( const rendering::PixelatedSettings& pixelationSettings )
{
	auto cameraEntities = _world.getEntitiesWith<ecs::Camera>();
	for (int i = 0; i < cameraEntities.size(); i++)
	{
		auto* camera = _world.getMutable<ecs::Camera>( cameraEntities[i] );
		camera->renderingSettings.pixelatedSettings = pixelationSettings;
	}
}

void EditorManager::setColorCorrectionSettings( const rendering::ColorCorrectionSettings& colorCorrectionSettings )
{
	auto cameraEntities = _world.getEntitiesWith<ecs::Camera>();
	for (int i = 0; i < cameraEntities.size(); i++)
	{
		auto* camera = _world.getMutable<ecs::Camera>( cameraEntities[i] );
		camera->renderingSettings.colorCorrectionSettings = colorCorrectionSettings;
	}
}

void EditorManager::setChromeAberrationSettings( const rendering::ChromeAberrationSettings& chromeAberrationSettings )
{
	auto cameraEntities = _world.getEntitiesWith<ecs::Camera>();
	for (int i = 0; i < cameraEntities.size(); i++)
	{
		auto* camera = _world.getMutable<ecs::Camera>( cameraEntities[i] );
		camera->renderingSettings.chromeAberrationSettings = chromeAberrationSettings;
	}
}

void EditorManager::setLightingSettings( const rendering::LightingSettings& lightingSettings )
{
	auto cameraEntities = _world.getEntitiesWith<ecs::Camera>();
	for (int i = 0; i < cameraEntities.size(); i++)
	{
		auto* camera = _world.getMutable<ecs::Camera>( cameraEntities[i] );
		camera->renderingSettings.lightingSettings = lightingSettings;
	}
}

void EditorManager::setClearSettings( const rendering::ClearSettings& clearSettings )
{
	auto cameraEntities = _world.getEntitiesWith<ecs::Camera>();
	for (int i = 0; i < cameraEntities.size(); i++)
	{
		auto* camera = _world.getMutable<ecs::Camera>( cameraEntities[i] );
		camera->renderingSettings.clearSettings = clearSettings;
	}
}

void EditorManager::LoadSceneNames( std::vector<std::string>& sceneNames )
{
	std::span<std::string> paths = _configReposiotry.getScenePaths();

	for (auto path : paths)
	{
		sceneNames.push_back( std::filesystem::path{ path }.filename().string() );
	}
}

void EditorManager::LoadScene( const std::string& sceneName, SceneData& scene )
{
	for (auto id : scene.sceneEntities) 
	{
		_world.destroyEntity( id );
	}

	clearEditorScene();
	ImportScene( sceneName, scene );
}

void EditorManager::SaveScene( const SceneData& scene, const std::string& sceneName )
{
	if (sceneName.find( ".json" ) == std::string::npos)
	{
		LOG_ERROR( "Scene name must have .json extension" );
		return;
	}

	auto scenePath = _configReposiotry.getScenePath( sceneName );
	ecs::ModelSerializer modelSerializer( _world, _resourceSystem );

	auto& entities = scene.sceneEntities;
	size_t count = entities.size();
	json sceneJson{};

	for (size_t i = 0; i < count; i++)
	{
		std::string name = "model" + std::to_string( i );
		json modelJson = modelSerializer.serialize( entities[i], name );

		sceneJson["entities"].push_back( modelJson );
	}

	assets::JsonFileSystem::SaveJson( sceneJson, scenePath.c_str() );
}

void EditorManager::SaveSceneAs( const scene::SceneData& scene, const std::string& sceneName )
{
	if (sceneName.find( ".json" ) == std::string::npos)
	{
		LOG_ERROR( "Scene name must have .json extension" );
		return;
	}

	auto scenePath = _configReposiotry.getScenePath( sceneName );
	ecs::ModelSerializer modelSerializer( _world, _resourceSystem );

	auto& entities = scene.sceneEntities;
	size_t count = entities.size();
	json sceneJson{};

	for (size_t i = 0; i < count; i++)
	{
		std::string name = "model" + std::to_string( i );
		json modelJson = modelSerializer.serialize( entities[i], name );

		sceneJson["entities"].push_back( modelJson );
	}
	assets::JsonFileSystem::SaveJson( sceneJson, scenePath.c_str() );
}

void EditorManager::ImportScene( const std::string& sceneName, SceneData& scene )
{
	auto scenePath = _configReposiotry.getScenePath( sceneName );
	json j = assets::JsonFileSystem::LoadJson( scenePath.c_str() );

	if (j.contains( "entities" ))
	{
		ecs::ModelSerializer modelSerializer( _world, _resourceSystem );

		const json& entities = j["entities"];
		for (auto& entity : entities)
		{
			std::vector<ComponentBuffer> buffers = modelSerializer.createComponentBuffers( entity );
			auto id = _world.spawn( buffers );
			scene.sceneEntities.push_back( id );
		}
		syncWithScene( scene );
	}
}

void EditorManager::ClearScene( SceneData& scene )
{
	clearEditorScene();
}

void EditorManager::NewScene( SceneData& scene )
{
	clearEditorScene();
}

id::EntityId EditorManager::getSelectedEntity() const
{
	return _selectedEntity;
}

std::span<id::EntityId> EditorManager::getSceneEntities() const
{
	return _world.getEntitiesWith<Transform>();
}

END_NAMESPACE1