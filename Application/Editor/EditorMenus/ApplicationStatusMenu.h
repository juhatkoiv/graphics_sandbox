#pragma once

#include "IMenu.h"

DECLARE1( class, editor, EditorManager );

BEGIN_NAMESPACE1( editor );

class ApplicationStatusMenu : public IMenu
{
public:
	ApplicationStatusMenu( EditorManager* editorManager );
	~ApplicationStatusMenu();

	void Draw( scene::SceneData& scene ) override;
};

END_NAMESPACE1