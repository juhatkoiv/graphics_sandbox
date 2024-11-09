#include "Precompiled/Precompiled.h"
#include "EditorMenus.h"

#include <vector>
#include <string>

#include "EditorRenderer.h"

#include "Editor/EditorManager.h"

#include "EditorMenus/SceneConfigMenu.h"
#include "EditorMenus/InstructionsMenu.h"
#include "EditorMenus/RenderingSettingsMenu.h"
#include "EditorMenus/RenderGraphMenu.h"

BEGIN_NAMESPACE1(editor)

EditorMenus::EditorMenus(EditorManager &editorManager) noexcept
	: _editorManager(editorManager)
{}

void EditorMenus::InitMenus(EditorRenderer &guiRenderer) noexcept
{
	guiRenderer.AddMenu(std::make_unique<SceneConfigMenu>(&_editorManager));
	guiRenderer.AddMenu(std::make_unique<InstructionsMenu>(&_editorManager));
	guiRenderer.AddMenu(std::make_unique<RenderingSettingsMenu>(&_editorManager));
	guiRenderer.AddMenu(std::make_unique<RenderGraphMenu>(&_editorManager));
}

EditorMenus::~EditorMenus() {}

END_NAMESPACE1