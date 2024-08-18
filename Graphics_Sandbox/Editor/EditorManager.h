#pragma once

#include <unordered_map>
#include <vector>
#include <span>

#include "ECS/EntitySpawnParams.h"
#include "ECS/Light.h"
#include "EditorTransform.h"
#include "Rendering/RenderEffect.h"
#include "ECS/World.h"
#include <Rendering/Serialization/RendererJsons.h>

DECLARE( class, string );
DECLARE1( struct, scene, SceneData );
DECLARE1( struct, ecs, Material );
DECLARE1( struct, ecs, Light );
DECLARE1( struct, ecs, Mesh );
DECLARE1( struct, ecs, EntityMoveParam );
DECLARE1( class, ecs, World );
DECLARE1( class, resources, ResourceSystem );
DECLARE1( class, appdata, ConfigRepository );

BEGIN_NAMESPACE1( editor )

class EditorManager
{
public:
	EditorManager( ecs::World& ecs, resources::ResourceSystem& resourceSystem, const appdata::ConfigRepository& configReposiotry );

	/* entities */
	void syncWithScene( scene::SceneData& scene );
	void updateEditorTransform( id::EntityId id, const EditorTransform& newTransform, ecs::EntityMoveParam& moveParam );
	void update();
	void moveEntity( const ecs::EntityMoveParam& moveParam );
	void setMaterial( id::EntityId id, const ecs::Material& material );
	void setLight( id::EntityId id, const ecs::Light& light );
	void setMesh( id::EntityId id, const ecs::Mesh& mash );
	void setEffect( id::EntityId id, rendering::RenderEffect effect );
	void setFullScreenEffect( rendering::PostProcessingEffects effect );
	void setSelected( id::EntityId id );
	id::EntityId getSelectedEntity() const;
	std::span<id::EntityId> getSceneEntities() const;

	template<typename T>
	bool hasComponent( id::EntityId id )
	{
		return _world.has<T>( id );
	}
	/* scenes */
	void NewScene( scene::SceneData& scene );
	void LoadSceneNames( std::vector<std::string>& scenes );
	void LoadScene( const std::string& sceneName, scene::SceneData& scene );
	void SaveScene( const scene::SceneData& scene, const std::string& sceneName );
	void SaveSceneAs( const scene::SceneData& scene, const std::string& sceneName );
	void ImportScene( const std::string& sceneName, scene::SceneData& scene );
	void ClearScene( scene::SceneData& scene );
	id::EntityId AddVoxelEntityToScene( ecs::EntitySpawnParam spawnParams, scene::SceneData& scene );
	id::EntityId AddLightToScene( ecs::EntitySpawnParam spawnParams, scene::SceneData& scene );
	id::EntityId addTransparentEntityToScene( ecs::EntitySpawnParam spawnParams, scene::SceneData& scene );

	/* components */
	EditorTransform& GetEditorTransform( id::EntityId id );
	ecs::Material& GetMaterialProperties( id::EntityId id );
	ecs::Light& GetLightProperties( id::EntityId id );
	ecs::Mesh& GetMeshProperties( id::EntityId id );
	void deleteSelectedEntity();

	Result<const std::string&> getMeshName( id::MeshId meshId );
	Result<const std::string&> getTextureName( id::TextureId textureId );

	/* rendering settings */
	void setBlurSettings( const rendering::FullScreenBlurSettings& blurSettings );
	void setPixelationSettings( const rendering::PixelatedSettings& pixelationSettings );
	void setColorCorrectionSettings( const rendering::ColorCorrectionSettings& colorCorrectionSettings );
	void setChromeAberrationSettings( const rendering::ChromeAberrationSettings& chromeAberrationSettings );
	void setLightingSettings( const rendering::LightingSettings& lightingSettings );
	void setClearSettings( const rendering::ClearSettings& clearSettings );

	/* object creation */
	void generateRandomizedVoxelTestScene( scene::SceneData& scene );
	void generateTerrain( scene::SceneData& scene );
	void generateRandomizedLights( scene::SceneData& scene );

	/**/
	const std::vector<RenderResourcesJson>& getRenderResources() const;
	const RenderGraphJson& getRenderGraph() const;
private:
	void clearEditorScene();
	void updateEditorTransform( id::EntityId id, const glm::vec3& position, const glm::vec3& scale );
	id::MeshId findMeshId( const std::string& meshId );

	SparseMap<id::EntityId, EditorTransform> _editorTransformLookup;
	id::EntityId _selectedEntity = id::InvalidEntityId;
	ecs::World& _world;
	resources::ResourceSystem& _resourceSystem;
	const appdata::ConfigRepository& _configReposiotry;
};

END_NAMESPACE1