#pragma once

#include "Editor/EditorMenus/IMenu.h"

DECLARE1( struct, scene, SceneData );
DECLARE1( class, editor, EditorManager );

BEGIN_NAMESPACE1(editor)


class RenderGraphMenu : public IMenu
{
public:
	RenderGraphMenu( EditorManager* editorManager );
	~RenderGraphMenu() {}
	void Draw( scene::SceneData& scene ) override;

};

END_NAMESPACE1