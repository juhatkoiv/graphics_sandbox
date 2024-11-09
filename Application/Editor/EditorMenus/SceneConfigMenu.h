#pragma once

#include "IMenu.h"

DECLARE1(class, editor, EditorManager);

BEGIN_NAMESPACE1(editor);

class SceneConfigMenu : public IMenu
{
public:
	SceneConfigMenu(EditorManager* editorManager);
	~SceneConfigMenu();

	void Draw(scene::SceneData& scene) override;
};

END_NAMESPACE1;