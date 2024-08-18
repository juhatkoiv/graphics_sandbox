#include "Precompiled/Precompiled.h"
#include "Editor/Editor.h"

#include "Scene/SceneData.h"
#include "Window/Window.h"

#include "Editor/EditorMenus.h"
#include "Editor/EditorManager.h"
#include "Editor/EditorStates.h"
#include "Editor/EditorRenderer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ECS/Renderer.h"
#include "ECS/World.h"

BEGIN_NAMESPACE1( editor )

using namespace ecs;

namespace
{
	scene::SceneData _scene;
}

Editor::Editor( window::Window& window, ecs::World& ecs, resources::ResourceSystem& resourceSystem, const appdata::ConfigRepository& configRepository )
	: _world( ecs )
{
	_editorManager.reset( new EditorManager( _world, resourceSystem, configRepository ) );
	_editorRenderer.reset( new EditorRenderer( window ) );
	_editorMenus.reset( new EditorMenus( *_editorManager ) );

	_editorMenus->InitMenus( *_editorRenderer );
}

Editor::~Editor()
{
}

void Editor::update( float deltaTime )
{
	if (_editorManager == nullptr)
		return;

	_editorManager->update();
}

void Editor::render( float deltaTime )
{
	if (_editorRenderer == nullptr)
		return;

	_editorRenderer->RenderEditorGUI( _scene );
}

END_NAMESPACE1