#pragma once

#include <vector>
#include <memory>

#include "EditorMenus.h"
#include "EditorMenus/IMenu.h"

DECLARE1(class, window, Window);
DECLARE1(struct, scene, SceneData);

BEGIN_NAMESPACE1(editor)

using MenuList = std::vector<std::shared_ptr<IMenu>>;

class EditorRenderer
{
public:
	EditorRenderer(window::Window &window);
	~EditorRenderer();

	void RenderEditorGUI(scene::SceneData& scene) const;

	void AddMenu(std::shared_ptr<IMenu> menu);
private:
	MenuList menus;
};

END_NAMESPACE1