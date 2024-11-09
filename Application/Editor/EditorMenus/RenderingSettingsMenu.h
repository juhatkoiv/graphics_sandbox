#pragma once

#include "IMenu.h"

DECLARE1(class, editor, EditorManager);

BEGIN_NAMESPACE1(editor);

class RenderingSettingsMenu : public IMenu
{
public:
	RenderingSettingsMenu(EditorManager* editorManager);
	~RenderingSettingsMenu();

	void Draw(scene::SceneData& scene) override;
};

END_NAMESPACE1;