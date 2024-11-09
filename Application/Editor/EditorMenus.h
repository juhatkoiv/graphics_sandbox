#pragma once

#include <vector>

#include "ECS/UniqueId.h"

DECLARE1(struct, scene, SceneData);
DECLARE1(class, window, Window);
DECLARE1(class, editor, EditorManager);
DECLARE1(class, editor, EditorRenderer);

BEGIN_NAMESPACE1(editor)

class EditorMenus
{
public:
	EditorMenus(EditorManager& editorManger) noexcept;
	~EditorMenus();

	void InitMenus(EditorRenderer &guiRenderer) noexcept;
	
private:
	EditorManager& _editorManager;
};

END_NAMESPACE1