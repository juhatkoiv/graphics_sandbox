#pragma once

#include "IMenu.h"

DECLARE1(class, editor, EditorManager);

BEGIN_NAMESPACE1(editor);

class InstructionsMenu : public IMenu
{
public:
	InstructionsMenu(EditorManager* editorManager);
	~InstructionsMenu() {}

	void Draw(scene::SceneData& scene) override;
};

END_NAMESPACE1;