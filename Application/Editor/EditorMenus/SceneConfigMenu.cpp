#include "Precompiled/Precompiled.h"
#include "SceneConfigMenu.h"

#include "ECS/ComponentIncludes.h"
#include "ECS/EntityMoveParam.h"
#include "ECS/EntitySpawnParams.h"
#include "Editor/EditorManager.h"
#include "Editor/EditorTransform.h"
#include "Scene/SceneData.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sstream>
#include <unordered_set>

BEGIN_NAMESPACE1( editor );

using namespace rendering;

class ImGuiTabBar
{
public:
	ImGuiTabBar( const char* tabName )
	{
		ImGui::BeginTabBar( tabName );
	}

	~ImGuiTabBar()
	{
		ImGui::EndTabBar();
	}
};

class ImGuiBegin
{
public:
	ImGuiBegin( const char* title )
	{
		ImGui::Begin( title );
	}

	~ImGuiBegin()
	{
		ImGui::End();
	}
};

#define IM_GUI_BEGIN(name, var_name) ImGuiBegin var_name(name);
#define IM_GUI_TAB_BAR(name, var_name) ImGuiTabBar var_name(name);

namespace
{
	static constexpr float PI = 3.14159265359f;

	SparseMap<id::EntityId, ecs::Material> _dirtyMaterialLookup;
	SparseMap<id::EntityId, ecs::Mesh> _dirtyMeshLookup;
	unsigned _selectedShaderId;
	EditorManager* _editorManager = nullptr;
	char _buffer[100];
	static std::vector<std::string> sceneNames;

	id::EntityId getSelectedEntity();
	bool IsEntitySelected();

	void RenderEntityTransform( scene::SceneData& scene );
	void RenderEntitySelection( scene::SceneData& scene );
	void RenderAddEntityToSceneButton( scene::SceneData& scene );
	void RenderMeshSelection();
	void RenderMaterialSelection( scene::SceneData& scene );
	bool SceneHasEntities( scene::SceneData& scene );
	void RenderSaveLoadSceneButtons( scene::SceneData& scene );
	void DrawImpl( scene::SceneData& scene );

	template<typename Type>
	static std::string GetTypeName( Type type )
	{
		std::string nameString( typeid(type).name() );

		size_t classSubStrIndex = nameString.find( "class " );
		if (classSubStrIndex != std::string::npos)
		{
			nameString.erase( classSubStrIndex, 7 );
		}

		size_t structSubStrIndex = nameString.find( "struct " );
		if (structSubStrIndex != std::string::npos)
		{
			nameString.erase( structSubStrIndex, 7 );
		}

		return nameString;
	}

	id::EntityId getSelectedEntity()
	{
		return _editorManager->getSelectedEntity();
	}

	bool IsEntitySelected()
	{
		return _editorManager->getSelectedEntity() != id::InvalidEntityId;
	}

	void setSelected( id::EntityId id )
	{
		_editorManager->setSelected( id );
	}

	void RenderEntityTransform( scene::SceneData& scene )
	{
		EditorTransform transform = {};
		auto selectedEntity = getSelectedEntity();

		if (selectedEntity != id::InvalidEntityId)
		{
			transform = _editorManager->GetEditorTransform( selectedEntity );
		}
		ImGui::Spacing();
		const bool positionChanged = ImGui::DragFloat3( "Entity Position", glm::value_ptr( transform.position ), 0.1f, -20.0, 20.0 );
		const bool rotationChanged = ImGui::DragFloat3( "Entity Rotation", glm::value_ptr( transform.angleScalars ), 0.1f, -PI, PI );
		const bool scaleChanged = ImGui::DragFloat3( "Entity Scale", glm::value_ptr( transform.scale ), 0.0, 0.1f, 10.0 );

		if (positionChanged || rotationChanged || scaleChanged)
		{
			ecs::EntityMoveParam deltaMove = {};
			_editorManager->updateEditorTransform( selectedEntity, transform, deltaMove );
			_editorManager->moveEntity( deltaMove );
		}
	}

	void RenderEntitySelection( scene::SceneData& scene )
	{
		ImGui::Text( "Select Entity" );

		_editorManager->syncWithScene( scene );

		auto selectedEntity = getSelectedEntity();
		std::stringstream selectedStream;
		selectedStream << selectedEntity;

		const auto& entities = _editorManager->getSceneEntities();

		if (ImGui::BeginCombo( "Selected Entity ", selectedStream.str().c_str() ))
		{
			for (int i = 0; i < entities.size(); i++)
			{
				std::stringstream ss;
				ss << entities[i];
				bool isSelected = (selectedEntity == entities[i]);
				if (ImGui::Selectable( ss.str().c_str(), isSelected ))
				{
					selectedEntity = entities[i];

					ImGui::SetItemDefaultFocus();
					setSelected( selectedEntity );
				}
			}
			ImGui::EndCombo();
		}
	}

	void RenderAddEntityToSceneButton( scene::SceneData& scene )
	{
		if (ImGui::Button( "Add Entity To Scene" ))
		{
			ecs::EntitySpawnParam param
			{
				.position = { 0,0,0 },
				.scale = { 1,1,1 },
				.rotationAxis = { 1,1,1 },
				.lightColor = { 1,1,1 },
				.meshColor = { 1,1,1,1 },
				.deltaAngle = 0.0,
			};
			_editorManager->AddVoxelEntityToScene( param, scene );
		}
	}
	
	void RenderAddTransparentEntityToSceneButton( scene::SceneData& scene ) 
	{
		if (ImGui::Button( "Add Transparent Entity To Scene" ))
		{
			ecs::EntitySpawnParam param
			{
				.position = { 0,0,0 },
				.scale = { 1,1,1 },
				.rotationAxis = { 1,1,1 },
				.lightColor = { 1,1,1 },
				.meshColor = { 1,1,1, 0.5 },
				.deltaAngle = 0.0,
			};
			_editorManager->addTransparentEntityToScene( param, scene );
		}
	}

	void RenderAddLightToSceneButton( scene::SceneData& scene )
	{
		if (ImGui::Button( "Add Light To Scene" ))
		{
			ecs::EntitySpawnParam param
			{
				.position = { 0,0,0 },
				.scale = { 0.2f,0.2f,0.2f },
				.rotationAxis = { 1,1,1 },
				.lightColor = { 1,1,1 },
				.meshColor = { 1,1,1,1 },
				.deltaAngle = 0.0,
			};
			_editorManager->AddLightToScene( param, scene );

		}
	}

	void RenderGenerateRandomizedVoxelTestSceneButton( scene::SceneData& scene )
	{
		if (ImGui::Button( "Generate voxel scene" ))
		{
			_editorManager->generateRandomizedVoxelTestScene( scene );
		}
	}

	void RenderGenerateLightsButton( scene::SceneData& scene )
	{
		if (ImGui::Button( "Generate random lights" ))
		{
			_editorManager->generateRandomizedLights( scene );

		}
	}
	void RenderGenerateTerrainButton( scene::SceneData& scene )
	{
		if (ImGui::Button( "Generate terrain scene" ))
		{
			_editorManager->generateTerrain( scene );
		}
	}

	void RenderMeshSelection()
	{
		if (!IsEntitySelected())
			return;

		auto selectedEntity = _editorManager->getSelectedEntity();
		if (!_editorManager->hasComponent<ecs::Mesh>( selectedEntity ))
			return;

		if (!_dirtyMeshLookup.has( selectedEntity ))
		{
			_dirtyMeshLookup[selectedEntity] = _editorManager->GetMeshProperties( selectedEntity );
		}

		ecs::Mesh& newMesh = _dirtyMeshLookup[selectedEntity];

		ImGui::Text( "%s", GetTypeName( newMesh ).c_str() );

		auto result = _editorManager->getMeshName( newMesh.meshId );
		if (result.success) 
		{
			const std::string& meshName = result.get();
			ImGui::Text( "%s", meshName.c_str());
		}
	}

	void RenderMaterialSelection( scene::SceneData& scene )
	{
		auto selectedEntity = getSelectedEntity();
		if (!_dirtyMaterialLookup.has( selectedEntity ))
		{
			_dirtyMaterialLookup[selectedEntity] = _editorManager->GetMaterialProperties( selectedEntity );
		}

		ecs::Material& newMaterial = _dirtyMaterialLookup[selectedEntity];
		ImGui::Text( "%s", GetTypeName( newMaterial ).c_str() );

		TextureType textureType = newMaterial.textureTypes;

		if (has( textureType, TextureType::Sprite ))
		{
			ImGui::TextUnformatted( "Sprite texture" );
		}

		if (has( textureType, TextureType::Diffuse ))
		{
			ImGui::TextUnformatted( "Diffuse texture" );
		}

		if (has( textureType, TextureType::Specular ))
		{
			ImGui::TextUnformatted( "Specular texture" );
		}

		ImGui::ColorEdit4( "Mesh Color", glm::value_ptr( newMaterial.meshColor ) );

		auto& shaderNames = shader::getShaderNames();

		if (ImGui::BeginCombo( "Entity Shader ", shader::getShaderName( newMaterial.shaderId ) )) // The second parameter is the label previewed before opening the combo.
		{
			for (int i = 0; i < shaderNames.size(); i++)
			{
				auto [id, name] = shaderNames[i];

				bool isSelected = (_selectedShaderId == id);
				if (ImGui::Selectable( name, isSelected ))
				{
					_selectedShaderId = id;
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
					else
					{
						_dirtyMaterialLookup[selectedEntity].shaderId = shader::getShaderId( name );
					}
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button( "Submit material changes" ))
		{
			_editorManager->setMaterial( selectedEntity, _dirtyMaterialLookup[selectedEntity] );
			_dirtyMaterialLookup.erase( selectedEntity );
		}

	}
	/*
	void RenderDeleteEntityButton( scene::SceneData& scene )
	{
		if (!IsEntitySelected())
			return;

		if (ImGui::Button( "Delete Entity" ))
		{
			_editorManager->deleteSelectedEntity();
		}
	}*/

	bool SceneHasEntities( scene::SceneData& scene )
	{
		const auto& sceneEntities = _editorManager->getSceneEntities();
		return !sceneEntities.empty();
	}

	void RenderSaveLoadSceneButtons( scene::SceneData& scene )
	{
		static std::string loadedSceneName;
		static std::string selectedSceneName;
		const char* title = selectedSceneName.empty() ? "No scene selected" : selectedSceneName.c_str();

		if (ImGui::Button( "New scene" ))
		{
			_editorManager->NewScene( scene );
		}

		if (ImGui::BeginCombo( "Scenes...", title ))
		{
			for (int i = 0; i < sceneNames.size(); i++)
			{
				std::stringstream ss;
				ss << sceneNames[i];
				bool isSelected = (selectedSceneName == sceneNames[i] && !sceneNames[i].empty()); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable( ss.str().c_str(), isSelected ))
				{
					selectedSceneName = sceneNames[i];
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}

		if (!selectedSceneName.empty())
		{
			if (ImGui::Button( "Load scene" ))
			{
				_editorManager->LoadScene( selectedSceneName, scene );
				loadedSceneName = selectedSceneName;
			}

			if (ImGui::Button( "Import scene" ))
			{
				_editorManager->ImportScene( selectedSceneName, scene );
			}
		}

		if (!loadedSceneName.empty())
		{
			if (ImGui::Button( "Save scene" ))
			{
				_editorManager->SaveScene( scene, loadedSceneName );
			}
		}

		if (ImGui::Button( "Clear scene" ))
		{
			_editorManager->ClearScene( scene );
		}

		static bool saveAs = false;
		if (ImGui::Button( "Save as..." ))
		{
			saveAs = true;
		}

		if (saveAs)
		{
			ImGui::Text( "Save name..." );
			if (ImGui::InputText( "##something", _buffer, IM_ARRAYSIZE( _buffer ) )) {}

			if (ImGui::Button( "Confirm Save" ))
			{
				_editorManager->SaveSceneAs( scene, _buffer );
				_editorManager->LoadSceneNames( sceneNames );
				saveAs = false;
			}
			ImGui::SameLine();
			if (ImGui::Button( "Cancel save as..." )) saveAs = false;

		}
	}

	void RenderLightPropertiesMenu( scene::SceneData& scene )
	{
		if (!IsEntitySelected())
			return;

		static SparseMap<id::EntityId, ecs::Light> _dirtyLightParam;
		auto selectedEntity = _editorManager->getSelectedEntity();

		if (!_editorManager->hasComponent<ecs::Light>( selectedEntity ))
			return;

		if (!_dirtyLightParam.has( selectedEntity ))
		{
			_dirtyLightParam[selectedEntity] = _editorManager->GetLightProperties( selectedEntity );
		}

		auto& light = _dirtyLightParam[selectedEntity];
		ImGui::TextUnformatted( "Light Properties" );
		ImGui::ColorEdit3( "Color", glm::value_ptr( light.lightColor ) );
		ImGui::DragFloat( "Intensity", &light.intensity, 0.1f, 0.0f, 10.0f );
		ImGui::DragFloat( "Attenuation", &light.attenuation, 0.1f, 0.0f, 10.0f );

		_editorManager->setLight( selectedEntity, light );
	}

	void DrawImpl( scene::SceneData& scene )
	{
		IM_GUI_BEGIN( "Scene Data", begin );
		IM_GUI_TAB_BAR( "Scene", sceneTabBar );

		if (ImGui::BeginTabItem( "Loading / Saving" ))
		{
			RenderSaveLoadSceneButtons( scene );
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem( "Generate Random" ))
		{
			RenderGenerateRandomizedVoxelTestSceneButton( scene );
			RenderGenerateLightsButton( scene );
			RenderGenerateTerrainButton( scene );
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem( "Entities" ))
		{
			RenderAddEntityToSceneButton( scene );
			RenderAddTransparentEntityToSceneButton( scene );
			RenderAddLightToSceneButton( scene );
			
			if (!SceneHasEntities( scene ))
			{
				ImGui::EndTabItem();
				return;
			}

			RenderEntitySelection( scene );

			if (!IsEntitySelected())
			{
				ImGui::EndTabItem();
				return;
			}

			RenderEntityTransform( scene );
			RenderMaterialSelection( scene );
			RenderMeshSelection();
			RenderLightPropertiesMenu( scene );

			ImGui::EndTabItem();
		}

	}
}

SceneConfigMenu::SceneConfigMenu( EditorManager* editorManager )
{
	assert( editorManager );

	_editorManager = editorManager;
	_editorManager->LoadSceneNames( sceneNames );
}

SceneConfigMenu::~SceneConfigMenu() {}

void SceneConfigMenu::Draw( scene::SceneData& scene )
{
	DrawImpl( scene );
}

END_NAMESPACE1;