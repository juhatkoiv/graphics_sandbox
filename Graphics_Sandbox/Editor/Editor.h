#pragma once

#include <memory>

#include "Application.h"

DECLARE1( class, editor, EditorManager );
DECLARE1( class, editor, EditorMenus );
DECLARE1( class, editor, EditorRenderer );
DECLARE1( class, window, Window );
DECLARE1( class, ecs, World );
DECLARE1( class, resources, ResourceSystem );
DECLARE1( class, appdata, ConfigRepository );

BEGIN_NAMESPACE1( editor )

class Editor : public Application
{
public:
	Editor( window::Window& window, ecs::World& ecs, resources::ResourceSystem& resourceSystem, const appdata::ConfigRepository& configRepository );
	~Editor();

	void update( float deltaTime ) override;
	void render( float deltaTime ) override;

private:

	std::unique_ptr<EditorMenus> _editorMenus;
	std::unique_ptr<EditorManager> _editorManager;
	std::unique_ptr<EditorRenderer> _editorRenderer;
	ecs::World& _world;
};

END_NAMESPACE1
